import numpy as np #import libraries 
import random

ROW_COUNT = 6 #variables
COLUMN_COUNT = 7
WINDOW_LENGTH = 4
PLAYER_PIECE = 2
BOT_PIECE = 1
EMPTY = 0

def create_board(): #create a basic board
    board = np.zeros((ROW_COUNT, COLUMN_COUNT))
    return board

def pretty_print_board(board): #add colors to the board j ==1 is yellow, player one and j == 2 is blue, player 2/computer
    flipped_board = np.flipud(board)

    print("\033[0;37;41m 0 \033[0;37;41m 1 \033[0;37;41m 2 \033[0;37;41m 3 \033[0;37;41m 4 \033[0;37;41m 5 \033[0;37;41m 6 \033[0m")
    for i in flipped_board:
        row_str = ""

        for j in i:
            if j == 1:
                #print(yellow)
                row_str +="\033[0;37;43m 1 "
            elif j ==2:
                row_str +="\033[0;37;44m 2 "
            else:
                #print black
                row_str +="\033[0;37;45m   "

        print(row_str+"\033[0m")

def get_valid_locations(board): #this loops through your columns and if it has an empty space, ie that column could recieve a move, then store this in the list of valid locations
    valid_locations = []
    for col in range(COLUMN_COUNT):
        if is_valid_location(board, col):
            valid_locations.append(col)
    return valid_locations

def is_valid_location(board, col): #you check if the topmost spot is empty
    return board[ROW_COUNT - 1][col] == 0

def get_next_open_row(board, col): #you find where the open is start from the bottom
    for r in range(ROW_COUNT):
        if board[r][col] == 0:
            return r

def drop_piece(board, row, col, piece):
    board[row][col] = piece

def score_position(board, piece):  #eval function
    score = 0

    # Score centre column
    centre_array = [int(i) for i in list(board[:, COLUMN_COUNT // 2])]
    centre_count = centre_array.count(piece)
    score += centre_count * 3

    # Score horizontal positions
    for r in range(ROW_COUNT):
        row_array = [int(i) for i in list(board[r, :])]
        for c in range(COLUMN_COUNT - 3):
            # Create a horizontal window of 4
            window = row_array[c:c + WINDOW_LENGTH]
            score += evaluate_window(window, piece)

    # Score vertical positions
    for c in range(COLUMN_COUNT):
        col_array = [int(i) for i in list(board[:, c])]
        for r in range(ROW_COUNT - 3):
            # Create a vertical window of 4
            window = col_array[r:r + WINDOW_LENGTH]
            score += evaluate_window(window, piece)

    # Score positive diagonals
    for r in range(ROW_COUNT - 3):
        for c in range(COLUMN_COUNT - 3):
            # Create a positive diagonal window of 4
            window = [board[r + i][c + i] for i in range(WINDOW_LENGTH)]
            score += evaluate_window(window, piece)

    # Score negative diagonals
    for r in range(ROW_COUNT - 3):
        for c in range(COLUMN_COUNT - 3):
            # Create a negative diagonal window of 4
            window = [board[r + 3 - i][c + i] for i in range(WINDOW_LENGTH)]
            score += evaluate_window(window, piece)

    return score

def evaluate_window(window, piece):
    score = 0
    # Switch scoring based on turn
    opp_piece = PLAYER_PIECE
    if piece == PLAYER_PIECE:
        opp_piece = BOT_PIECE

    # Prioritise a winning move
    # Minimax makes this less important
    if window.count(piece) == 4:
        score += 100
    # Make connecting 3 second priority
    elif window.count(piece) == 3 and window.count(EMPTY) == 1:
        score += 5
    # Make connecting 2 third priority
    elif window.count(piece) == 2 and window.count(EMPTY) == 2:
        score += 2
    # Prioritise blocking an opponent's winning move (but not over bot winning)
    # Minimax makes this less important
    if window.count(opp_piece) == 3 and window.count(EMPTY) == 1:
        score -= 4

    return score

def winning_move(board, piece): #this just checks if the game is over, but it runs after each move
    # Check valid horizontal locations for win
    for c in range(COLUMN_COUNT - 3):
        for r in range(ROW_COUNT):
            if board[r][c] == piece and board[r][c + 1] == piece and board[r][c + 2] == piece and board[r][c + 3] == piece:
                return True

    # Check valid vertical locations for win
    for c in range(COLUMN_COUNT):
        for r in range(ROW_COUNT - 3):
            if board[r][c] == piece and board[r + 1][c] == piece and board[r + 2][c] == piece and board[r + 3][c] == piece:
                return True

    # Check valid positive diagonal locations for win
    for c in range(COLUMN_COUNT - 3):
        for r in range(ROW_COUNT - 3):
            if board[r][c] == piece and board[r + 1][c + 1] == piece and board[r + 2][c + 2] == piece and board[r + 3][c + 3] == piece:
                return True

    # check valid negative diagonal locations for win
    for c in range(COLUMN_COUNT - 3):
        for r in range(3, ROW_COUNT):
            if board[r][c] == piece and board[r - 1][c + 1] == piece and board[r - 2][c + 2] == piece and board[r - 3][c + 3] == piece:
                return True
            
def is_terminal_node(board): #Before implementing the minimax algorithm, the two game-terminating states need to be defined as terminal nodes. If there is a winning move from either player, or if the board fills up without a win (leading to a draw), the game will end.
    return winning_move(board, PLAYER_PIECE) or winning_move(board, BOT_PIECE) or len(get_valid_locations(board)) == 0

def minimax(board, depth, alpha, beta, maximisingPlayer): #sex a b pruning
    valid_locations = get_valid_locations(board)

    is_terminal = is_terminal_node(board)
    if depth == 0 or is_terminal:
        if is_terminal:
            # Weight the bot winning really high
            if winning_move(board, BOT_PIECE):
                return (None, 9999999)
            # Weight the human winning really low
            elif winning_move(board, PLAYER_PIECE):
                return (None, -9999999)
            else:  # No more valid moves
                return (None, 0)
        # Return the bot's score
        else:
            return (None, score_position(board, BOT_PIECE))

    if maximisingPlayer:
        value = -9999999
        # Randomise column to start
        column = random.choice(valid_locations)
        for col in valid_locations:
            row = get_next_open_row(board, col)
            # Create a copy of the board
            b_copy = board.copy()
            # Drop a piece in the temporary board and record score
            drop_piece(b_copy, row, col, BOT_PIECE)
            new_score = minimax(b_copy, depth - 1, alpha, beta, False)[1]
            if new_score > value:
                value = new_score
                # Make 'column' the best scoring column we can get
                column = col
            alpha = max(alpha, value)
            if alpha >= beta:
                break
        return column, value

    else:  # Minimising player
        value = 9999999
        # Randomise column to start
        column = random.choice(valid_locations)
        for col in valid_locations:
            row = get_next_open_row(board, col)
            # Create a copy of the board
            b_copy = board.copy()
            # Drop a piece in the temporary board and record score
            drop_piece(b_copy, row, col, PLAYER_PIECE)
            new_score = minimax(b_copy, depth - 1, alpha, beta, True)[1]
            if new_score < value:
                value = new_score
                # Make 'column' the best scoring column we can get
                column = col
            beta = min(beta, value)
            if alpha >= beta:
                break
        return column, value

def main():
    board = create_board()
    game_over = False
    turn = 0  # 0 for Player 1, 1 for Bot

    while not game_over:
        # Player 1's turn (Human)
        if turn == 0:
            col = int(input("Player 1, make your selection (0-6): "))
            if is_valid_location(board, col):
                row = get_next_open_row(board, col)
                drop_piece(board, row, col, PLAYER_PIECE)

                if winning_move(board, PLAYER_PIECE):
                    print("Player 1 wins!")
                    game_over = True

        # Bot's turn
        else:
            # Set depth to a reasonable level for the minimax algorithm
            depth = 4  # You can adjust this value
            col, minimax_score = minimax(board, depth, -np.inf, np.inf, True)
            if is_valid_location(board, col):
                row = get_next_open_row(board, col)
                drop_piece(board, row, col, BOT_PIECE)

                if winning_move(board, BOT_PIECE):
                    print("Bot wins!")
                    game_over = True

        pretty_print_board(board)
        turn += 1
        turn = turn % 2

        if game_over:
            # Optionally add a pause or wait for user input before closing
            input("Press Enter to close.")

if __name__ == "__main__":
    main()