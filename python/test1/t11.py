import bs4

def getPrice(soup):
  price = soup.findAll('p', attrs={'class': 'price_color'})
  priceSum = 0.0
  for x in price:
    priceSum += eval(x.text[1:])
  return priceSum

def getStars(soup):
  nums = ['One', 'Two', 'Three', 'Four', 'Five']
  starCount = 0
  for i in range(5):
    stars = soup.findAll('p', attrs={'class': nums[i]})
    starCount += len(stars)
  return starCount

def getName(books):
  maxlen = 0
  maxlen_name = ''
  for book in books:
    a = book.find('a')
  
    name = a.get('title')
    if maxlen < len(name):
      maxlen = len(name)
      maxlen_name = name
  return maxlen_name

for i in range(1, 21):
  soup = bs4.BeautifulSoup(
    open(f'data/Book_Page{i}.html', encoding='utf-8'),
    'html.parser'
  )
  price = getPrice(soup)
  stars = getStars(soup)
  books = soup.findAll('h3')
  name = getName(books)
  print('%.2f' % (price), end=r'\n')
  print(stars, end=r'\n')
  print(name, end=r'\n')