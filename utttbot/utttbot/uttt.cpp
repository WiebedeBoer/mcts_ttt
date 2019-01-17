// uttt.cpp
// Aswin van Woudenberg

#include "uttt.h"

//operator
std::ostream &operator<<(std::ostream& os, const Player &p) {
	if (p == Player::None) {
		os << ".";
	} else if (p == Player::X) {
		os << "X";
	} else if (p == Player::O) {
		os << "O";
	} else if (p == Player::Active) {
		os << "*";
	}
	return os;
}

//operator
std::ostream &operator<<(std::ostream& os, const State &s) {
	for (int r=0; r<9; r++) {
		for (int c=0; c<9; c++) {
			os << s.board[r][c];
		}
		os << std::endl;
	}
	os << std::endl;
	for (int r=0; r<3; r++) {
		for (int c=0; c<3; c++) {
			os << s.macroboard[r][c];
		}
		os << std::endl;
	}
	return os;
}

//operator
std::ostream &operator<<(std::ostream& os, const Move &m) {
	os << m.x << " " << m.y;
	return os;
}

//player state
Player getCurrentPlayer(const State &state)
{
	int countX = 0;
	int countO = 0;
	for (int r=0; r<9; r++) {
		for (int c=0; c<9; c++) {
			if (state.board[r][c] == Player::X) countX++;
			else if (state.board[r][c] == Player::O) countO++;
		}
	}
	return (countX > countO ? Player::O : Player::X); 
}

//winner
Player getWinner(const State &state, int row, int col)
{
	for (int r=0; r<3; r++)
		if (state.board[row*3+r][col*3] == state.board[row*3+r][col*3+1] && 
				state.board[row*3+r][col*3+1] == state.board[row*3+r][col*3+2] && 
				state.board[row*3+r][col*3+2] != Player::None) 
			return state.board[row*3+r][col*3];
	for (int c=0; c<3; c++)
		if (state.board[row*3][col*3+c] == state.board[row*3+1][col*3+c] && 
				state.board[row*3+1][col*3+c] == state.board[row*3+2][col*3+c] && 
				state.board[row*3][col*3+c] != Player::None) 
			return state.board[row*3][col*3+c];
	if (state.board[row*3][col*3] == state.board[row*3+1][col*3+1] && 
			state.board[row*3+1][col*3+1] == state.board[row*3+2][col*3+2] && 
			state.board[row*3][col*3] != Player::None) 
		return state.board[row*3][col*3];
	if (state.board[row*3][col*3+2] == state.board[row*3+1][col*3+1] && 
			state.board[row*3+1][col*3+1] == state.board[row*3+2][col*3] && 
			state.board[row*3][col*3+2] != Player::None) 
		return state.board[row*3][col*3+2];
	for (int r=0; r<3; r++)
		for (int c=0; c<3; c++)
			if (state.board[row*3+r][col*3+c] == Player::None)
				return Player::Active;
	return Player::None;
}

//move
State doMove(const State &state, const Move &m)
{
	State result = state;

	if (state.macroboard[m.y/3][m.x/3] != Player::Active) {
		return result; // Invalid move
	}

	result.board[m.y][m.x] = getCurrentPlayer(state);
	for (int r=0; r<3; r++) {
		for (int c=0; c<3; c++) {
			result.macroboard[r][c] = getWinner(result, r, c);
		}
	}

	int empty = 0;
	for (int r=0; r<9; r++)
		for (int c=0; c<9; c++)
			if (result.board[r][c] == Player::None)
				empty++;
	bool stillPlaying = empty > 0 && getWinner(result) == Player::None;
	if (result.macroboard[m.y%3][m.x%3] == Player::Active)
		for (int r=0; r<3; r++)
			for (int c=0; c<3; c++)
				if ((r!=m.y%3 || c!=m.x%3 || !stillPlaying) && result.macroboard[r][c] == Player::Active)
					result.macroboard[r][c] = Player::None;

	return result; 
}

//winner
Player getWinner(const State &state)
{
	for (int r=0; r<3; r++)
		if (state.macroboard[r][0] == state.macroboard[r][1] && 
				state.macroboard[r][1] == state.macroboard[r][2] && 
				state.macroboard[r][2] != Player::None && 
				state.macroboard[r][2] != Player::Active) 
			return state.macroboard[r][2];
	for (int c=0; c<3; c++)
		if (state.macroboard[0][c] == state.macroboard[1][c] && 
				state.macroboard[1][c] == state.macroboard[2][c] && 
				state.macroboard[2][c] != Player::None && 
				state.macroboard[2][c] != Player::Active) 
			return state.macroboard[2][c];
	if (state.macroboard[0][0] == state.macroboard[1][1] && state.macroboard[1][1] == state.macroboard[2][2] && 
			state.macroboard[0][0] != Player::None && state.macroboard[0][0] != Player::Active) 
		return state.macroboard[0][0];
	if (state.macroboard[0][2] == state.macroboard[1][1] && state.macroboard[1][1] == state.macroboard[2][0] && 
			state.macroboard[0][2] != Player::None && state.macroboard[0][2] != Player::Active) 
		return state.macroboard[0][2];
	return Player::None;
}

//move
std::vector<Move> getMoves(const State &state)
{
	std::vector<Move> moves;
	if (getWinner(state) == Player::None) {
		for (int r=0; r<9; r++) {
			for (int c=0; c<9; c++) {
				if (state.macroboard[r/3][c/3] == Player::Active && state.board[r][c] == Player::None) {
					moves.push_back(Move{c,r});
				}
			}
		}
	}
	return moves;
}

//trials
unsigned const n_trials = 1000;
unsigned const mc_match = 1;
unsigned const mc_other = 1;
enum class PlayerType { Human, Computer };
//to implement mctrial, board player
State mcTrial(const State &board)
{
	State trialboard = State(board);
	//State::const_iterator boardIte;
	std::vector<Move> moves;
	std::vector<Move>::iterator move;
	//std::array<int, 9> subscores = {0,0,0,0,0,0,0,0,0};

	while (1) {
		moves = getMoves(trialboard);

		if (moves.size() == 0) {
			return trialboard; //no more moves
		}

		do {
			moves = getMoves(trialboard);
			move = select_randomly<std::vector<Move>::iterator>(moves.begin(), moves.end()); //moves on the trial
		} while (trialboard[*move] != Player::None);

		trialboard = doMove(trialboard, *move);
	}
	return board;//bord return
}

//to implement update scores, scores board player
void mcUpdateScores(std::array<int, 9> &scores, const State &board, const Player &player)
{
	//State trialboard = State(board);
	//State::const_iterator boardIte;
	//const_iterator boardIte;
	size_t count = 0; //counter
	std::array<int, 9> boardIte = { 0,0,0,0,0,0,0,0,0 };
	//std::array<int, 9> subscores = { 0,0,0,0,0,0,0,0,0 };

	//if winner is player, score up player
	if (getWinner(board) == player) {
		for (int k = 0; k < 9; k++) {
			for (boardIte[k] = board.macroboard.begin(); boardIte[k] != board.macroboard.end(); boardIte++) {
				if (boardIte[k] == Player::None);
				else if (boardIte[k] == player)
					scores[count] += mc_match;
				else
					scores[count] -= mc_other;
				count++; //increment score
			}
		}
	}
	//else score up not player
	else {
		for (int k = 0; k < 9; k++) {
			for (boardIte[k] = board.macroboard.begin(); boardIte[k] != board.macroboard.end(); boardIte++) {
				if (boardIte == Player::None);
				else if (boardIte[k] == player)
					scores[count] -= mc_match;
				else
					scores[count] += mc_other;
				count++; //increment score
			}
		}
	}
}

//to implement get best move, board scores
Move getBestMove(const std::array<int, 9> &scores, const State &board)
{
	int highScore = -999; //at 1000 trials, minus one
	Move highMove;
	//State::const_iterator boardIte;
	std::array<int, 9> boardIte = { 0,0,0,0,0,0,0,0,0 };
	//std::array<int, 9> subscores = { 0,0,0,0,0,0,0,0,0 };
	//const_iterator boardIte;
	std::array<int, 9>::const_iterator scoreIte = scores.begin();

	//loop through moves
	//for (boardIte[] = board.macroboard.begin(); boardIte[] != board.macroboard.end(); boardIte++) {
	for (int k =0; k <9; k++) {
		for (boardIte[k] = board.macroboard.begin(); boardIte[k] != board.macroboard.end(); boardIte++) {
			if (*scoreIte > highScore && boardIte == Player::None) {
				highScore = *scoreIte;
				highMove = boardIte[k] - board.macroboard.begin(); //at which iteration
			}
			scoreIte++;
		}

	}

	return highMove; //return the move with highest score
}

//to implement move, board player trials
Move mcMove(const State &board, const Player &player)
{
	//scores array for trial board
	std::array<int, 9> scoreboard = {
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
	};

	for (unsigned i = 0; i < n_trials; i++) {
		const State trialboard = mcTrial(board);
		mcUpdateScores(scoreboard, trialboard, player); //score updating
	}

	return getBestMove(scoreboard, board); //best move
}


