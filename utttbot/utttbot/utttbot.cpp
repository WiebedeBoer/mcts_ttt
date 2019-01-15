// utttbot.cpp
// Aswin van Woudenberg

#include "utttbot.h"

#include <iostream>
#include <sstream>
//#include <vector> vector?

void UTTTBot::run() {
	std::string line;
	while (std::getline(std::cin, line)) {
		std::vector<std::string> command = split(line, ' ');
		if (command[0] == "settings") {
			setting(command[1], command[2]);
		} else if (command[0] == "update" && command[1] == "game") {
			update(command[2], command[3]);
		} else if (command[0] == "action" && command[1] == "move") {
			move(std::stoi(command[2]));
		} else {
			std::cerr << "Unknown command: " << line << std::endl;
		}
	}
}

//random move
void UTTTBot::move(int timeout) {
	// Do something more intelligent here than return a random move
	std::vector<Move> moves = getMoves(state);
	//std::cout << "place_disc " << *select_randomly(moves.begin(), moves.end()) << std::endl; //random move

	if (State.board(4,4)) {
		std::cout << "place_disc " << *select_randomly(4,4) << std::endl;
	}
	else {
		std::cout << "place_disc " << *select_randomly(moves.begin(), moves.end()) << std::endl; //random move
	}

	/*if you first player, give up center board
	//for (inc = 0; inc <9; inc++){
	if (firstplayer ==me){
	if (moves <8){
	std::cout << "place_disc " << *select_randomly(5) << std::endl; //center board first 8 moves
	//}
	}
	else if (moves >=8 && moves <16){
	//wherever opponent chooses to move
	std::cout << "place_disc " << *select_randomly(moveboard) << std::endl; //other board next 8 moves
	}
	else if (moves >=16 && moves <24){
	//wherever opponent chooses to move opposite board
	moveoppositeboard = 10 - moveboard;
	std::cout << "place_disc " << *select_randomly(moveoppositeboard) << std::endl; 
	//other opposite board next 8 moves till win
	}
	else {
	//use random trial moves monte carlo
	//MonteCarloTreeSearch()
	}
	}
	else {
	//use random trial moves monte carlo
	//MonteCarloTreeSearch()
	}
	
	*/



}

//update, leave this
void UTTTBot::update(std::string &key, std::string &value) {
	if (key == "round") {
		round = std::stoi(value);
	} else if (key == "field") {
		int row = 0;
		int col = 0;
		std::vector<std::string> fields = split(value, ',');
		for (std::string &field : fields) {
			if (field == "0") {
				state.board[row][col] = Player::X; 
			} else if (field == "1") {
				state.board[row][col] = Player::O;
			} else {
				state.board[row][col] = Player::None;
			}
			col++;
			if (col == 9) {
				row++; 
				col = 0;
			}
		}
	} else if (key == "macroboard") {
		int row = 0;
		int col = 0;
		std::vector<std::string> fields = split(value, ',');
		for (std::string &field : fields) {
			if (field == "-1") {
				state.macroboard[row][col] = Player::Active;
			} else if (field == "0") {
				state.macroboard[row][col] = Player::X;
			} else if (field == "1") {
				state.macroboard[row][col] = Player::O;
			} else {
				state.macroboard[row][col] = Player::None;
			}
			col++;
			if (col == 3) {
				row++;
				col = 0;
			}
		}
	}
}

//settings
void UTTTBot::setting(std::string &key, std::string &value) {
	if (key == "timebank") {
		timebank = std::stoi(value);
	} else if (key == "time_per_move") {
		time_per_move = std::stoi(value);
	} else if (key == "player_names") {
		std::vector<std::string> names = split(value, ',');
		player_names[0] = names[0];
		player_names[1] = names[1];
	} else if (key == "your_bot") {
		your_bot = value;
	} else if (key == "your_botid") {
		your_botid = std::stoi(value);
	}
}

//string
std::vector<std::string> UTTTBot::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

//node
/*
public class Node {
	State state;
	Node parent;
	List<Node> childArray;
	// setters and getters
}
public class Tree {
	Node root;
}
*/

/*
public class State {
	Board board;
	int playerNo;
	int visitCount;
	double winScore;

	// copy constructor, getters, and setters

	public List<State> getAllPossibleStates() {
		// constructs a list of all possible states from current state
	}
	public void randomPlay() {
		// get a list of all possible positions on the board and  play a random move 
	}
}
*/

/*
public class MonteCarloTreeSearch {
	static final int WIN_SCORE = 10;
	int level;
	int opponent;

	public Board findNextMove(Board board, int playerNo) {
		// define an end time which will act as a terminating condition

		opponent = 3 - playerNo;
		Tree tree = new Tree();
		Node rootNode = tree.getRoot();
		rootNode.getState().setBoard(board);
		rootNode.getState().setPlayerNo(opponent);

		while (System.currentTimeMillis() < end) {
			Node promisingNode = selectPromisingNode(rootNode);
			if (promisingNode.getState().getBoard().checkStatus()
			  == Board.IN_PROGRESS) {
				expandNode(promisingNode);
			}
			Node nodeToExplore = promisingNode;
			if (promisingNode.getChildArray().size() > 0) {
				nodeToExplore = promisingNode.getRandomChildNode();
			}
			int playoutResult = simulateRandomPlayout(nodeToExplore);
			backPropogation(nodeToExplore, playoutResult);
		}

		Node winnerNode = rootNode.getChildWithMaxScore();
		tree.setRoot(winnerNode);
		return winnerNode.getState().getBoard();
	}
}
*/

//selection
/*
private Node selectPromisingNode(Node rootNode) {
	Node node = rootNode;
	while (node.getChildArray().size() != 0) {
		node = UCT.findBestNodeWithUCT(node);
	}
	return node;
}
*/

//uct
/*
public class UCT {
	public static double uctValue(
	  int totalVisit, double nodeWinScore, int nodeVisit) {
		if (nodeVisit == 0) {
			return Integer.MAX_VALUE;
		}
		return ((double) nodeWinScore / (double) nodeVisit)
		  + 1.41 * Math.sqrt(Math.log(totalVisit) / (double) nodeVisit);
	}

	public static Node findBestNodeWithUCT(Node node) {
		int parentVisit = node.getState().getVisitCount();
		return Collections.max(
		  node.getChildArray(),
		  Comparator.comparing(c -> uctValue(parentVisit,
			c.getState().getWinScore(), c.getState().getVisitCount())));
	}
}
*/

//expand
/*
private void expandNode(Node node) {
	List<State> possibleStates = node.getState().getAllPossibleStates();
	possibleStates.forEach(state -> {
		Node newNode = new Node(state);
		newNode.setParent(node);
		newNode.getState().setPlayerNo(node.getState().getOpponent());
		node.getChildArray().add(newNode);
	});
}
*/

//prop
/*
private void backPropogation(Node nodeToExplore, int playerNo) {
	Node tempNode = nodeToExplore;
	while (tempNode != null) {
		tempNode.getState().incrementVisit();
		if (tempNode.getState().getPlayerNo() == playerNo) {
			tempNode.getState().addScore(WIN_SCORE);
		}
		tempNode = tempNode.getParent();
	}
}
private int simulateRandomPlayout(Node node) {
	Node tempNode = new Node(node);
	State tempState = tempNode.getState();
	int boardStatus = tempState.getBoard().checkStatus();
	if (boardStatus == opponent) {
		tempNode.getParent().getState().setWinScore(Integer.MIN_VALUE);
		return boardStatus;
	}
	while (boardStatus == Board.IN_PROGRESS) {
		tempState.togglePlayer();
		tempState.randomPlay();
		boardStatus = tempState.getBoard().checkStatus();
	}
	return boardStatus;
}
*/

//board
/*
public class Board {
	int[][] boardValues;
	public static final int DEFAULT_BOARD_SIZE = 3;
	public static final int IN_PROGRESS = -1;
	public static final int DRAW = 0;
	public static final int P1 = 1;
	public static final int P2 = 2;

	// getters and setters
	public void performMove(int player, Position p) {
		this.totalMoves++;
		boardValues[p.getX()][p.getY()] = player;
	}

	public int checkStatus() {
		//Evaluate whether the game is won and return winner.
		// If it is draw return 0 else return -1 
	}

	public List<Position> getEmptyPositions() {
		int size = this.boardValues.length;
		List<Position> emptyPositions = new ArrayList<>();
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (boardValues[i][j] == 0)
					emptyPositions.add(new Position(i, j));
			}
		}
		return emptyPositions;
	}
}
*/

//empty
/*
public void givenEmptyBoard_whenSimulateInterAIPlay_thenGameDraw() {
	Board board = new Board();
	int player = Board.P1;
	int totalMoves = Board.DEFAULT_BOARD_SIZE * Board.DEFAULT_BOARD_SIZE;
	for (int i = 0; i < totalMoves; i++) {
		board = mcts.findNextMove(board, player);
		if (board.checkStatus() != -1) {
			break;
		}
		player = 3 - player;
	}
	int winStatus = board.checkStatus();

	assertEquals(winStatus, Board.DRAW);
}
*/