def create_piece(board, pos=[0, 0]):
    x, y = pos
    def piece(dir, step):
        nonlocal x, y
        nx, ny = x, y
        dx, dy = dir
        for _ in range(step):
            nx += dx
            ny += dy
            if (nx < 0 or nx > n-1 or ny < 0 or ny > n-1):
                return 'Wrong move'
            if (board[ny][nx] == 'x'):
                return 'Wrong move'
        x, y = nx, ny
        return [x, y]
    return piece
n, start_x, start_y = map(int, input().split())

# 读取棋盘
board = []
for _ in range(n):
    row = input().strip().split()
    board.append(row)
    
# 创建棋子
piece = create_piece(board, [start_x, start_y])

# 处理移动指令
try:
    while True:
        cmd = input().strip()
        dx, dy, step = map(int, cmd.split())
        result = piece([dx, dy], step)
        print(result)
except EOFError:
    pass  # 读取到EOF时正常退出