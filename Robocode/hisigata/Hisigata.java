package hisigata;
import robocode.*;
//import java.awt.Color;

// API help : http://robocode.sourceforge.net/docs/robocode/robocode/Robot.html

/**
 * Hisigata - a robot by (your name here)
 */
public class Hisigata extends Robot
{

	/**
	 * run: Hisigata's default behavior
	 */
	public void run() {
		// Initialization of the robot should be put here

		// After trying out your robot, try uncommenting the import at the top,
		// and the next line:

		// setColors(Color.red,Color.blue,Color.green); // body,gun,radar

		// Robot main loop
			double height = getBattleFieldHeight();
			double width  = getBattleFieldWidth();
		
			
			if (getX()>width/2){
				
				turnRight(270-getHeading());
				ahead(getX()-width/2);
				turnRight(90);
				ahead(height - getY());

			}
			else if(getX()<width/2){
				
				turnLeft(getHeading()-90);
				ahead(width/2-getX());
				turnLeft(90);
				ahead(height - getY());
			}
			//1 ni mukau made
			
		while(true) {
			// Replace the next 4 lines with any behavior you would like
		}
	}

	/**
	 * onScannedRobot: What to do when you see another robot
	 */
	public void onScannedRobot(ScannedRobotEvent e) {
		// Replace the next line with any behavior you would like
		
	}

	/**
	 * onHitByBullet: What to do when you're hit by a bullet
	 */
	public void onHitByBullet(HitByBulletEvent e) {
		// Replace the next line with any behavior you would like
		back(10);
	}
	
	/**
	 * onHitWall: What to do when you hit a wall
	 */
	public void onHitWall(HitWallEvent e) {
		// Replace the next line with any behavior you would like
		
		double height = getBattleFieldHeight();
		double width  = getBattleFieldWidth();
		double y = getY();
		double x = getX();
		
		double θ = Math.atan(height/width) * 180 / Math.PI;
		//kakudo wo motomeru
		
		
		if (y>height / 1.1){
			turnLeft(getHeading());
			turnRight(90 + θ);
			ahead(10000); 
		}
		//1 ni irutoki
		else if (x>width / 1.1){
			turnRight(180 - 2 * θ);
			ahead(1000);
		}
		//2 ni irutoki
		else if (y<height / 11){
			turnRight(2 * θ);
			ahead(1000);

		}
		//3 ni irutoki 
		else if (x<width / 11){
			turnRight(180 - 2 * θ);
			ahead(1000);

		}
		//4 ni irutoki
	}	
}

//seihoukei dato kirei
			










					