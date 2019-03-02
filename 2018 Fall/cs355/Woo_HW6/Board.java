import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
//import java.io.Console; //Enable this if you would like to print on the console for debugging

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.Timer;

public class Board extends JPanel 
        implements ActionListener {
	/*define constants for customizing the game */
	public  final int BOARD_WIDTH = 10; //number of rows in the board
	public  final int BOARD_HEIGHT = 20; //number of columns
	public final int CELL_WIDTH = 30;
	public final int CELL_HEIGHT = 35;	
    public  final char SPACE = ' ';
    public  final String TARGET = "355";
    
    private final int DELAY = 400;
    private static final Color BLACK =  new Color(0,0,0);
    
    private Timer timer;  //We use the java.util.Timer to create a game cycle. The shapes move on a square by square basis (not pixel by pixel).
    private boolean isFallingFinished = false; //isFallingFinished is set to true when the fall of a piece is completed; it is set to false when the next piece starts to fall. 
    private boolean isStarted = false;  //isStarted is set to false when the board height reaches the limit and the next piece can't be moved (i.e., game is over). The program exist when isStarted becomes false. 
    private boolean isPaused = false;   //set to true when the game is paused; false otherwise.
    private JLabel statusbar; 
    private JLabel scorebar;  //The label displaying the game score
    private JLabel scoretext; 
    private Font textFont;
    private Piece curPiece;   //The current (falling) piece
    private char [][] board; //The board grid
    private boolean [][] checked;
    private player cplayer;
    
    public Board(Tetris parent) {
    	cplayer=new player();
        initBoard(parent);
        statusbar.setText(""+cplayer.getpieces());
        scorebar.setText(""+cplayer.getscore());
    }
    
    /*clear all cells on the board. A cell in the board is considered empty if the character at that cell is SPACE. Iniatially all cells are initialized with SPACE.*/
    private void clearBoard() {
        for (int i = 0; i < BOARD_HEIGHT; i++) {
        	for (int j = 0; j < BOARD_WIDTH; j++) {
        		board[j][i] = SPACE;
        	}
        }
    }
    private void clearcheck() {
        for (int i = 0; i < BOARD_HEIGHT; i++) {
        	for (int j = 0; j < BOARD_WIDTH; j++) {
        		checked[j][i]=false;
        	}
        }
    }//clear the boolean values that indicate if a board square is checked
    
    /*calculate the cell width and height in pixels*/
    private int squareWidth() { return (int) getSize().getWidth() / BOARD_WIDTH; }
    private int squareHeight() { return (int) getSize().getHeight() / BOARD_HEIGHT; }
    
    /*calculate the X coordinate of the left upper corner of j th cell on a row. (j is a 0-based index)*/  
    private int calcX(int j) {
    	return (j * squareWidth());
        
    }
    /*calculate the Y coordinate of the left upper corner of i th cell on a column. (i is a 0-based index)*/  
    private int calcY(int i) {
        Dimension size = getSize();
    	int boardTop = (int) size.getHeight() - BOARD_HEIGHT * squareHeight(); 
    	return (boardTop + i * squareHeight());
        
    }
    /*initialize the game board, create the first piece, and start the timer */
    private void initBoard(Tetris parent) {
        
       setFocusable(true);
       curPiece = new Piece();
       //create the Timer. The board object is registered as the listener  for the timer. When the timer is fired, Board's action listener method will be called. 
       timer = new Timer(DELAY, this);
       timer.start(); 

   	   this.textFont = new Font("Helvatica", Font.BOLD, 22);
       statusbar = parent.getStatusBar(); //get the reference to the Tetris's status bar
       scorebar = parent.getScoreBar();   //get the reference to the Tetris's score bar
       scoretext = parent.getScoreText();   //get the reference to the Tetris's score bar
       
       scoretext.setFont(textFont);
       scorebar.setBorder(BorderFactory.createEtchedBorder(1));
       scorebar.setFont(textFont);
       statusbar.setFont(textFont);
       
       addKeyListener(new TAdapter());
       //create and clear the board
       board = new char [BOARD_WIDTH] [BOARD_HEIGHT];
       checked = new boolean [BOARD_WIDTH][BOARD_HEIGHT];
       clearBoard();
       clearcheck();
       
    }

    /* When Timer is fired, Board's override of actionPerformed will be called*/
    @Override
    public void actionPerformed(ActionEvent e) {
        /*if the falling of the current piece is completed, create a new piece*/
        if (isFallingFinished) {        
            isFallingFinished = false;
            createNewPiece();
        } else{ // else move the current piece one level down on the board.     
            oneLineDown();
        }
    }

/* Start the game. */  
    public void start()  {
        
        if (isPaused)
            return;

        isStarted = true;
        isFallingFinished = false;
        
        createNewPiece();
        timer.start();
        clearBoard();
    }
    /* Pause the game. */  
    private void pause()  {
        
        if (!isStarted)
            return;

        isPaused = !isPaused;
        
        if (isPaused) {
            
            timer.stop();
            statusbar.setText("PAUSED");
        } else {
            
            timer.start();
            statusbar.setText(" ");
        }
        
        repaint();
    }

    /* draw a square over the non-empty board cells (a cell is non-empty if its value is not SPACE). The color of the cell is determined based on the cell value. */
    private void drawSquare(Graphics g, int x, int y, char number)  {
        
        Color colors[] = { new Color(50, 150, 50), new Color(204, 102, 102), 
            new Color(102, 204, 102), new Color(102, 102, 204), 
            new Color(204, 204, 102), new Color(204, 102, 204), 
            new Color(0, 255, 0), new Color(0, 0, 255),
            new Color(102, 204, 204), new Color(218, 170, 0)
        };

        Color color = colors[Character.getNumericValue(number)];

        g.setColor(color);
        g.fillRect(x + 1, y + 1, squareWidth() - 2, squareHeight() - 2);
        g.setColor(BLACK);
        g.setFont(textFont);
        g.drawString(String.valueOf(number), x+(squareWidth()-(CELL_WIDTH/2)+1)/2, y+(squareHeight()+(CELL_HEIGHT/2))/2 );
             
        g.setColor(color.brighter());
        g.drawLine(x, y + squareHeight() - 1, x, y);
        g.drawLine(x, y, x + squareWidth() - 1, y);

        g.setColor(color.darker());
        g.drawLine(x + 1, y + squareHeight() - 1,
                         x + squareWidth() - 1, y + squareHeight() - 1);
        g.drawLine(x + squareWidth() - 1, y + squareHeight() - 1,
                         x + squareWidth() - 1, y + 1);

    }

    /*draw the squares for the non empty cells on the board (already dropped pieces and the current piece currently falling)*/
    private void doDrawing(Graphics g) {       
   	
    	/*draw the squares for already dropped pieces*/
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            for (int j = 0; j < BOARD_WIDTH; j++) {
                if (board[j][i] != SPACE) {
                	drawSquare(g, calcX(j),calcY(i), board[j][i]);
                }
            }
        }
        /*draw the square for the currently dropping piece*/
        if (curPiece.getNumber() != SPACE) {                
                drawSquare(g, calcX(curPiece.getX()),calcY(curPiece.getY()), curPiece.getNumber());
            
        }        
    }

    /*Board's overwrite for paintComponent. This will be called every time "repaint() is called . */
    @Override
    public void paintComponent(Graphics g) { 

        super.paintComponent(g);
        doDrawing(g);
    }

    /*move the current piece all the way down on the board*/
    private void dropDown() {
        
        int newY = curPiece.getY();
        
        while (newY < BOARD_HEIGHT-1) {
            
            if (!tryMove(curPiece, 0 , 1))
                break;
            ++newY;
        }
        
        pieceDropped();
    }
   
    /*move the current piece one row down on the board*/
    private void oneLineDown()  {
        
        if (!tryMove(curPiece, 0, 1))
            pieceDropped();
    }


    /*checks if the fall of the current piece is completed. 
     * **TODO** If so, updates the board and the score.*/
    private void pieceDropped() {                
        updateBoard(curPiece); 

        if (!isFallingFinished)
            createNewPiece();
    }

    /*creates the new piece; if the board is full (i.e, can't move the current piece) stops the game. */  
    private void createNewPiece()  {
        
        curPiece.setRandomNumber();
        curPiece.setX(BOARD_WIDTH / 2 + 1);
        curPiece.setY(0);

        if (!tryMove(curPiece, 0, 0)) {
            
            curPiece.setNumber(SPACE);
            timer.stop();
            isStarted = false;
            statusbar.setText("GAME OVER");
        }
    }

    /*moves the current piece by newX,newY on the Board and repaints the Board*/
    private boolean tryMove(Piece newPiece, int newX, int newY) {
        int x = newPiece.getX() + newX;
        int y = newPiece.getY() + newY;
           
        if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT)
        	return false;     
        
        if (board[x][y] != SPACE)
            return false;
       

        curPiece = newPiece;
        curPiece.setX(x);
        curPiece.setY(y);

        repaint();

        return true;
    }
    
    /*Updates the Board: 
     * clears all cell groups including 3 (or more) matching adjacent cells
     * clears all cell groups including the target characters 
     *     - can appear either horizontal(left to right or right to left )  or vertical (up to down or down to up)
     *     - the characters of the target string need to appear in the same order.*/
    public void updateBoard(Piece curPiece) {
        
    	board[curPiece.getX()][curPiece.getY()] = curPiece.getNumber();
    	boolean doremove=checktarget(curPiece.getX(), curPiece.getY());
    	int is3=check3(curPiece.getNumber(), curPiece.getX(), curPiece.getY());
    	if(is3>2){
    		removep(curPiece.getNumber(), curPiece.getX(), curPiece.getY());
    		alldown();
    	}
    	else if(doremove) {
    		board[curPiece.getX()][curPiece.getY()] = SPACE;
    		alldown();
    	}
    	clearcheck();
    	puyo();
        statusbar.setText(""+cplayer.getpieces());
        scorebar.setText(""+cplayer.getscore());
    	/*TODO*/
        /*Update the Board: 
         * clear all cell groups including 3 (or more) matching adjacent cells
         * clear all cell groups including the target characters */

    	
     }
    
    private boolean checktarget(int x, int y) {
    	boolean removeit=false;
    	if(x>1) {
    		if((""+board[x][y]+board[x-1][y]+board[x-2][y]).equals("355")||(""+board[x][y]+board[x-1][y]+board[x-2][y]).equals("553")) {
    			board[x-1][y]=SPACE;
    			board[x-2][y]=SPACE;
    			cplayer.addpieces(2);
    			cplayer.addscore(30);
    			removeit=true;
    		}
    	}
    	if(x>0&&x<BOARD_WIDTH-1) {
    		if((""+board[x+1][y]+board[x][y]+board[x-1][y]).equals("355")||(""+board[x+1][y]+board[x][y]+board[x-1][y]).equals("553")) {
    			board[x+1][y]=SPACE;
    			board[x-1][y]=SPACE;
    			cplayer.addpieces(2);
    			cplayer.addscore(30);
    			removeit=true;
    		}
    	}
    	if(x<BOARD_WIDTH-2) {
    		if((""+board[x][y]+board[x+1][y]+board[x+2][y]).equals("355")||(""+board[x][y]+board[x+1][y]+board[x+2][y]).equals("553")) {
    			board[x+1][y]=SPACE;
    			board[x+2][y]=SPACE;
    			cplayer.addpieces(2);
    			cplayer.addscore(30);
    			removeit=true;
    		}
    	}
    	if(y<BOARD_HEIGHT-2) {
    		if((""+board[x][y]+board[x][y+1]+board[x][y+2]).equals("355")||(""+board[x][y]+board[x][y+1]+board[x][y+2]).equals("553")) {
    			board[x][y+1]=SPACE;
    			board[x][y+2]=SPACE;
    			cplayer.addpieces(2);
    			cplayer.addscore(30);
    			removeit=true;
    		}
    	}
    	return removeit;
    }
    
    //check is there are three same blocks
    private int check3(char it, int x, int y) {
    	if(x<0||x>=BOARD_WIDTH) return 0;
    	if(y<0||y>=BOARD_HEIGHT) return 0;
    	if(it!=board[x][y]) return 0;
    	if(checked[x][y]) return 0;
    	checked[x][y]=true;
    	int count=1;
    	count+=check3(it, x-1, y);
    	count+=check3(it, x+1, y);
    	count+=check3(it, x, y-1);
    	count+=check3(it, x, y+1);
    	return count;
    }
    
    //remove blocks of same char
    private void removep(char it, int x, int y) {
    	if(x<0||x>=BOARD_WIDTH) return;
    	if(y<0||y>=BOARD_HEIGHT) return;
    	if(it!=board[x][y]) return;
    	board[x][y]=SPACE;
    	cplayer.addpieces(1);
		cplayer.addscore(5);
    	removep(it, x-1, y);
    	removep(it, x+1, y);
    	removep(it, x, y-1);
    	removep(it, x, y+1);
    	return;
    } 

    private void alldown() {
    	for(int i=0; i<BOARD_WIDTH; i++) {
    		for(int j=BOARD_HEIGHT-1; j>0; j--) {
    			if(board[i][j]==SPACE) {
    				board[i][j]=board[i][j-1];
    				board[i][j-1]=SPACE;
    			}
    		}
    		boolean turn1=false;
    		boolean turn2=false;
    		for(int j=0; j<BOARD_HEIGHT; j++) {
    			if(board[i][j]!=SPACE) turn2=true;
    			if(board[i][j]==SPACE) {
    				if(turn2){
    					turn1=true;
    					break;
    				}
    			}
    		}
    		if(turn1) i--;
    	}
    }//if there is a hole in the column, then fill the hole.
    
    private void puyo() {
        for (int i = 0; i < BOARD_HEIGHT; i++) {
        	for (int j = 0; j < BOARD_WIDTH; j++) {
        		if(board[j][i]==SPACE) continue;
        		boolean doremove=checktarget(j, i);
        		if(check3(board[j][i], j, i)>2) {
        			removep(board[j][i], j, i);
            		alldown();
            		i=0;
            		j=0;
            	}
        		else if(doremove) {
        			board[j][i]=SPACE;
        			alldown();
        			i=0;
        			j=0;
        		}
        		clearcheck();
        	}
        }
    }//if condition is matching after removal, remove those blocks also.
    
    class player{
    	int score=0;
    	int pieces=0;
    	
    	public int getscore() {return score;}
    	public int getpieces() {return pieces;}
    	public void addscore(int s) {score+=s;}
    	public void addpieces(int p) {pieces+=p;}
    }//player class that stores n of removed pieces and score
    
    /* handles the key presses*/
    class TAdapter extends KeyAdapter {
        
         @Override
         public void keyPressed(KeyEvent e) {

             if (!isStarted || curPiece.getNumber() == SPACE) {  
                 return;
             }

             int keycode = e.getKeyCode();

             if (keycode == 'P') {
                 pause();
                 return;
             }

             if (isPaused)
                 return;

             switch (keycode) {
                 
             case KeyEvent.VK_LEFT:
                 tryMove(curPiece, - 1, 0);
                 break;
                 
             case KeyEvent.VK_RIGHT:
                 tryMove(curPiece,  1, 0);
                 break;
                 
             case KeyEvent.VK_DOWN:
            	 tryMove(curPiece, 0, 2);
                 break;
                 
             case KeyEvent.VK_UP:
                 tryMove(curPiece, 0, 0);
                 break;
                 
             case KeyEvent.VK_SPACE:
                 dropDown();
                 break;
                 
             case 'D':
                 oneLineDown();
                 break;
             }
         }
     }
}
