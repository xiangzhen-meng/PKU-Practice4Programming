exit = None
class TaggedList:  #元素带标签的列表
  def __init__(self, sc, tg):
    self.score = sc
    self.tag = tg
    self.len = len(self.score)
  def __len__(self):
    return self.len
  def __contains__(self, item):
    return item in self.score
  def __str__(self):
    ret = ''
    for i in range(self.len):
      ret += f'{self.tag[i]}:{self.score[i]},'
    return ret
  def __getitem__(self, key):
    if isinstance(key, int):
      return self.score[key]
    else:
      idx = self.tag.index(key)
      return self.score[idx]
  def __setitem__(self, key, value):
    if isinstance(key, int):
      self.score[key] = value
      return value
    else:
      idx = self.tag.index(key)
      self.score[idx] = value
      return value
    
a = TaggedList([70,80,90,100],["语文","数学","英语","物理"])
print(len(a),78 in a, 80 in a) #>>4 False True
print(str(a)) #>>语文:70,数学:80,英语:90,物理:100,
print(a[0],a['数学']) #>>70 80   标签也可以作为下标访问元素
a[1] = a['物理'] = 85
print(a) #>>语文:70,数学:85,英语:90,物理:85,