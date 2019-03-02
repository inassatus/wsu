import java.util.Random;
import java.util.concurrent.TimeUnit;

public class Piece {

	/*the character (number) value of the piece*/
    private char number;
    private int curX = 0; //the current X coordinate of the  piece
    private int curY = 0; //the current Y coordinate of the  piece

    public Piece() {
        setNumber('0'); //default number 
    }
    /* get and set methods for "number"*/
    public void setNumber(char newNumber) {
        this.number = newNumber;
    }
    public char getNumber()  { 
    	return number; 
    }
    public void setX(int newX){
    	this.curX = newX;
    }
    public void setY(int newY){
    	this.curY = newY;
    }
    public int getX(){
    	return this.curX;
    }
    public int getY(){
    	return this.curY;
    }
    /*sets a random digit to the cell character*/
    public void setRandomNumber() {
        
        Random r = new Random();
        int x = Math.abs(r.nextInt()) % 10;
        setNumber(Character.forDigit(x, 10));
    }
    
    
}
