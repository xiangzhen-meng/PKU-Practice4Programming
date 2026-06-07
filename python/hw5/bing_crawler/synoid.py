import re
import requests  # request库用于获取网络资源
from bs4 import BeautifulSoup
import sys

CHROME_PATH = "/Applications/Google Chrome.app/Contents/MacOS/Google Chrome"
USER_DATA_DIR = "."

def getHtmlByPyppeteer(url):
  import asyncio
  import pyppeteer as pyp

  async def asGetHtml(url):
    browser = await pyp.launch(
      executablePath=CHROME_PATH,
      userDataDir=USER_DATA_DIR,
      headless=False)
    page = await browser.newPage()
    await page.setUserAgent(
      'Mozilla/5.0 (Windows NT 6.1; Win64; \
      x64) AppleWebKit/537.36 (KHTML, like Gecko) \
      Chrome/78.0.3904.70 Safari/537.36')
    await page.evaluateOnNewDocument(
      '() =>{ Object.defineProperties(navigator, \
      { webdriver:{ get: () => false } }) }')
    await page.goto(url)
    text = await page.content()
    await browser.close()
    return text

  loop = asyncio.new_event_loop()
  asyncio.set_event_loop(loop)
  return loop.run_until_complete(asGetHtml(url))
  # 返回值就是asGetHtml(url)的返回值

def getHtml(url):  #用requests库获取网址url的网页
  fakeHeaders = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)  \
    AppleWebKit/537.36 (KHTML, like Gecko)  \
    Chrome/81.0.4044.138 Safari/537.36 Edg/81.0.416.77',
    'Accept': 'text/html,application/xhtml+xml,*/*'
  }
  #用于伪装浏览器发送请求
  try:
    r = requests.get(url,headers = fakeHeaders)
    r.encoding = r.apparent_encoding  #确保网页编码正确
    return r.text  #返回值是个字符串，内含整个网页内容
  except Exception as e:
    print(e)
    return None

def getBingWords(word):  #下载n个百度图片搜来的关于word的图片保存到本地
  url = "https://cn.bing.com/dict/search?q="
  url += word
  url += "&FORM=BDVSP6&cc=cn"
  html = getHtml(url)  #用requests获取网页
  html = getHtmlByPyppeteer(url) #用ppppeteer获取网页
  
  soup = BeautifulSoup(html, 'html.parser')
  div = soup.find(id="synoid")
  target = div.get_text('', strip=True)   # 只要文本
  # print(target)
  # print(str(div))                        # 整个 div 的 HTML
  # print(div.decode_contents())         # 只要 div 内部的 HTML
  lst = target.split(',')
  dot = lst[0].rfind('.')
  lst[0] = lst[0][dot+1:]
  # print(lst)
  output = "\n".join(lst)
  output = '$' + word + '\n' + output
  return output

def getBingPicture(word):
  url = "https://cn.bing.com/dict/search?q="
  url += word
  url += "&FORM=BDVSP6&cc=cn"
  html = getHtml(url)  #用requests获取网页
  html = getHtmlByPyppeteer(url) #用ppppeteer获取网页

  soup = BeautifulSoup(html, 'html.parser')
  div = soup.find(attrs={'class': 'simg'})
  img = div.find('img')
  picurl = img.get('src')
  try:
    r = requests.get(picurl, stream=True)
    f = open('./python/bing_crawler/hw5/{0}.jpg'.format(word), "wb")
    f.write(r.content)
    f.close()
  except Exception as e:
    pass
  

if __name__ == "__main__":
  inp = open('./python/hw5/bing_crawler/words.txt', 'r')
  otp = open('./python/hw5/bing_crawler/result1.txt', 'w')
  sys.stdin = inp
  sys.stdout = otp
  while True:
    try:
      word = input()
    except EOFError:
      break
    
    try:
      synoid = getBingWords(word)
      print(synoid)
      getBingPicture(word)
    except Exception as e:
      print('$' + word)
      continue

  inp.close()
  otp.close()