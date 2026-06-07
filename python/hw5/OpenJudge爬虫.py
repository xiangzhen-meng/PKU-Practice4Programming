import asyncio
import getpass
import os
import pyppeteer as pyp
#import playwright

CHROME_PATH = "/Applications/Google Chrome.app/Contents/MacOS/Google Chrome"
USER_DATA_DIR = "./.pyppeteer-user-data"

async def antiAntiCrawler(page): #为page添加反反爬虫手段
	await page.setUserAgent('Mozilla/5.0 (Windows NT 6.1; \
		Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) \
    		Chrome/78.0.3904.70 Safari/537.36')
	await page.evaluateOnNewDocument(
		'() =>{ Object.defineProperties(navigator, \
		{ webdriver:{ get: () => false } }) }')
        
async def getOjSourceCode(loginUrl):
    width, height = 1400, 800  #网页宽高
    browser = await pyp.launch(headless=False,
                               executablePath=CHROME_PATH,
                               userDataDir = USER_DATA_DIR,
                               args=[f'--window-size={width},{height}'])
    email = os.environ.get("OPENJUDGE_EMAIL") or input("OpenJudge email: ")
    password = os.environ.get("OPENJUDGE_PASSWORD") or getpass.getpass("OpenJudge password: ")
    page = await browser.newPage()
    await antiAntiCrawler(page)
    await page.setViewport({'width': width, 'height': height})
    await page.goto(loginUrl)
    element = await page.querySelector("#email")  #寻找帐号输入框
    if element is None:
        raise RuntimeError("没有找到帐号输入框 #email")
    await element.type(email)  		#输入帐号（邮箱）
    element = await page.querySelector("#password") #寻找密码输入框
    if element is None:
        raise RuntimeError("没有找到密码输入框 #password")
    await element.type(password)  			#输入密码
    element = await page.querySelector(
              "#main > form > div.user-login > p:nth-child(2) > button")
    if element is None:
        raise RuntimeError("没有找到登录按钮")
    await asyncio.gather(
        page.waitForNavigation(),
        element.click()
    )
    print("after login:", page.url)
    if "/auth/login" in page.url:
        message = await page.querySelector(".login-message")
        if message is not None:
            obj = await message.getProperty("innerText")
            text = await obj.jsonValue()
            raise RuntimeError("登录失败: " + text)
        raise RuntimeError("登录失败，仍停留在登录页")
    await page.waitForSelector("#main h2",
            timeout=30000)   #等待"正在进行的比赛...."标题出现
    element = await page.querySelector('#userMenu a[href*="/user/"]')
    #找"个人首页”链接
    if element is None:
        raise RuntimeError("没有找到个人首页链接")
    await asyncio.gather(
        page.waitForNavigation(),
        element.click()
    )
    print("profile page:", page.url)
    # page.waitForNavigation(waitUntil='networkidle2') #稳定等待时间
    elements = await page.querySelectorAll(".result-right")
    #找所有"Accepted"链接, 其有属性 class="result-right"
    if not elements:
        print("没有找到 Accepted 代码链接 .result-right")
        await browser.close()
        return
    page2 = await browser.newPage() 	#新开一个页面 (标签)
    await antiAntiCrawler(page2)
    for element in elements[:12]: 	#只打印前两个程序
        obj = await element.getProperty("href") #获取href属性
        url = await obj.jsonValue()
        await page2.goto(url)      #在新页面(标签)中装入新网页
        element = await page2.querySelector("pre") #查找pre tag
        if element is None:
            print("没有找到代码 pre:", url)
            continue
        obj = await element.getProperty("innerText")
        text = await obj.jsonValue()
        print(text)
        print("-------------------------")
    await browser.close()
    
def main():
    url = "http://openjudge.cn/auth/login/"
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    loop.run_until_complete(getOjSourceCode(url))
    #old asyncio.get_event_loop().run_until_complete(getOjSourceCode(url))

if __name__ == "__main__":
    main()
