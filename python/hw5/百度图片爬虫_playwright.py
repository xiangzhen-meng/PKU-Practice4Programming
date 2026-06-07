from playwright.sync_api import sync_playwright
import requests
import os


# from urllib.parse import unquote
# s = input()
# print(unquote(s, "utf-8"))
#
# exit()
#2023.12.03测过都可以，但是pyppetter版本有api过时和不稳定问题
import re
import requests  # request库用于获取网络资源

CHROME_PATH = "/Applications/Google Chrome.app/Contents/MacOS/Google Chrome"

def getHtmlByPyppeteer(url):
	import asyncio
	import pyppeteer as pyp
	async def asGetHtml(url):
		browser = await pyp.launch(headless=False)
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
	return  loop.run_until_complete(asGetHtml(url))
	# 返回值就是asGetHtml(url)的返回值

def getHtml(url):  #用requests库获取网址url的网页
	fakeHeaders = {'User-Agent':
		   'Mozilla/5.0 (Windows NT 10.0; Win64; x64)  \
		   AppleWebKit/537.36 (KHTML, like Gecko)  \
		   Chrome/81.0.4044.138 Safari/537.36 Edg/81.0.416.77',
			'Accept': 'text/html,application/xhtml+xml,*/*'
	}
	#用于伪装浏览器发送请求
	try:
		r = requests.get(url,headers = fakeHeaders)
		r.encoding = r.apparent_encoding	#确保网页编码正确
		return r.text	#返回值是个字符串，内含整个网页内容
	except Exception as e:
		print(e)
		return None

def getHtmlByPlayWright(url):
		# browser = await pyp.launch(headless=False)
		# page = await browser.newPage()
		# await page.setUserAgent(
		# 	'Mozilla/5.0 (Windows NT 6.1; Win64; \
        #     x64) AppleWebKit/537.36 (KHTML, like Gecko) \
        #     Chrome/78.0.3904.70 Safari/537.36')
		# await page.evaluateOnNewDocument(
		# 	'() =>{ Object.defineProperties(navigator, \
        #     { webdriver:{ get: () => false } }) }')
		# await page.goto(url)
		# text = await page.content()
		# await browser.close()
		# return text
	with sync_playwright() as p:
		print("sssss")
		browser = p.chromium.launch(
			executable_path=CHROME_PATH, headless=False)
		print(browser)
		page = browser.new_page()

		print("new")
		page.goto(url)
		print("url=",url)
		page.wait_for_timeout(10000)  # 等待页面加载
		print("url=", url)
		text = page.content()
		page.wait_for_timeout(10000)  # 等待页面加载
		browser.close()
		return text


#####
	# with sync_playwright() as p:
	# 	browser = p.chromium.launch(headless=False)
	# 	page = browser.new_page()
	# 	page.on('response', handle_response)
	# 	# 打开快手用户页面，这里替换成你要抓取的快手用户页面地址
	# 	page.goto('https://www.kuaishou.com/profile/3xx8xjrbuknq7qg')
	# 	page.wait_for_timeout(10000)  # 等待页面加载
	# 	browser.close()
def getBaiduPictures(word,n):	#下载n个百度图片搜来的关于word的图片保存到本地
	url = "https://image.baidu.com/search/index?tn=baiduimage&ipn=r&ct=201326592&cl=2&lm=-1&st=-1&fm=index&fr=&hs=0&xthttps=111111&sf=1&fmq=&pv=&ic=0&nc=1&z=&se=1&showtab=0&fb=0&width=&height=&face=0&istype=2&ie=utf-8&word="
	url += word
	#html = getHtml(url)  #用requests获取网页
	html = getHtmlByPlayWright(url) #用ppppeteer获取网页
	pt = '\"thumbURL\":.*?\"(.*?)\"' #正则表达式，用于寻找图片url

	pt = r'{"thumbURL":"(http[^"]+)"'

	i = 0
	for x in re.findall(pt, html):	#x就是图片url
		x = x.lower()
		print(x)
		try:
			r = requests.get(x, stream=True)#获取x对应的网络资源
			f = open('{0}{1}.jpg'.format(word,i),
                                           "wb") #"wb"表示二进制写方式打开文件
			f.write(r.content)    #图片内容写入文件
			f.close()
			i = i + 1
		except Exception as e :
			pass
		if i >= n:
			break



if __name__ == "__main__":
	#print(getHtmlByPyppeteer("https://quote.eastmoney.com/sh600000.html"))
	getBaiduPictures("猫", 5)
	getBaiduPictures("熊猫", 5)


