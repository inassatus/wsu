/*
Part of the code for drawing the game board has beed adapted from http://zetcode.com/tutorials/javagamestutorial/tetris/  
*/

import java.awt.BorderLayout;
import java.awt.EventQueue;
import java.awt.Font;
import java.awt.GridLayout;

import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.BorderFactory;

/*

 */
public class Tetris extends JFrame {

	private JLabel statusbar; // Displays the game status: "PAUSED" or "GAME OVER"
    private JLabel scorebar;  // Displays the current score
	private JLabel scoretext; 
 
    

    public Tetris() {
    	/* initialize the game UI */
        initUI(); 
    }

    private void initUI() {
    	/* create a panel including the status and score labels */
    	JPanel panel=new JPanel ();
        statusbar = new JLabel(" ");
        scorebar = new JLabel("0");
        scoretext = new JLabel("Score");
        
       
        
        /* add borders to the labels*/
        scoretext.setBorder(BorderFactory.createEtchedBorder(1)); 
        scorebar.setBorder(BorderFactory.createEtchedBorder(1));

        
        /*add the labels to the panels. The panel layout is a 2x2 grid. */
        panel.setLayout(new GridLayout(2,2));
        panel.add(statusbar);
        panel.add(new JLabel(""));
        panel.add(scoretext);
        panel.add(scorebar); 
        
        /* add the panel to the bottom of the main frame */
        add(panel, BorderLayout.SOUTH);

        /* create the game board and add it to the main frame */
        Board board = new Board(this);
        board.setBorder(BorderFactory.createBevelBorder(1));
        add(board);
        board.start(); //start the game 

        setTitle("Tetris");
        setSize(board.BOARD_WIDTH * board.CELL_WIDTH, board.BOARD_HEIGHT * board.CELL_HEIGHT);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
    }

    public JLabel getStatusBar() {

        return statusbar;
    }

    public JLabel getScoreBar() {

        return scorebar;
    }

    public JLabel getScoreText() {

        return scoretext;
    }
    
    public static void main(String[] args) {

        EventQueue.invokeLater(() -> {

            Tetris game = new Tetris();
            game.setVisible(true);
        });
    }
}
