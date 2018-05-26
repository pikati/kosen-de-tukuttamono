package ad;
import robocode.*;
//import java.awt.Color;

// API help : http://robocode.sourceforge.net/docs/robocode/robocode/Robot.html

/**
 * Advanced - a robot by (your name here)
 */
public class Advanced extends AdvancedRobot
{
	/**
	 * run: Advanced's default behavior
	 */

	
	public void run() {
		// Initialization of the robot should be put here

		// After trying out your robot, try uncommenting the import at the top,
		// and the next line:

		// setColors(Color.red,Color.blue,Color.green); // body,gun,radar

		setAdjustGunForRobotTurn(true);
		setAdjustRadarForRobotTurn(true);
		

		// Robot main loop
		while(true) {
			// Replace the next 4 lines with any behavior you would like
			double height = getBattleFieldHeight();
			double width  = getBattleFieldWidth();
			double x = getX();
			double y = getY();
			
			if (x > width / 2 && y > height / 2){
			
				if (getHeading()>180){
					turnLeft(getHeading() - 180);
					ahead(y - height / 2);
					out.println("migiue");
				}
				else if (getHeading()<180){
					turnRight(180 - getHeading());
					ahead(y - height / 2);
					out.println("migiue");
				}
				
				turnRight(90);
				out.println("mawaru");
				
				ahead(x - width / 2);
				out.println("aaaaaa");
				
				turnRight(getHeading());
				out.println("nya-n");
				
				}
				
			else if (x < width / 2 && y > height / 2){
			
				if (getHeading()>180){
					turnLeft(getHeading() - 180);
					ahead(y - height / 2);
					out.println("hidariue");
				}
				else if (getHeading()<180){
					turnRight(180 - getHeading());
					ahead(y - height / 2);
					out.println("hidariue");
				}
				
				turnLeft(90);
				out.println("mawaru");
				
				ahead(width / 2 - x);
				out.println("aaaaaa");
				
				turnRight(getHeading());
				out.println("nya-n");
				
				}

			else if (x > width / 2 && y < height / 2){
			
				if (getHeading()>180){
					turnLeft(360 - getHeading());
					ahead(y - height / 2);
					out.println("migisita");
				}
				else if (getHeading()<180){
					turnLeft(getHeading());
					ahead(y - height / 2);
					out.println("migisita");
				}
				
				turnLeft(90);
				out.println("mawaru");
				
				ahead(width / 2 - x);
				out.println("aaaaaa");
				
				turnRight(getHeading());
				out.println("nya-n");
				
				}
				
			else if (x < width / 2 && y < height / 2){
			
				if (getHeading()>180){
					turnRight(360 - getHeading());
					ahead(height / 2 - y);
					out.println("hidarisita");
				}
				else if (getHeading()<180){
					turnRight(getHeading());
					ahead(height / 2 - y);
					out.println("hidarisita");
				}
				
				turnRight(90);
				out.println("mawaru");
				
				ahead(width / 2 - x);
				out.println("aaaaaa");
				
				turnRight(getHeading());
				out.println("nya-n");
				
				}
			
			ahead(100);
			setAhead(10000);
			turnRight(10000);
			//tanjento de 45do ni iku
			
		}
	}

	/**
	 * onScannedRobot: What to do when you see another robot
	 */
	public void onScannedRobot(ScannedRobotEvent e) {
		// Replace the next line with any behavior you would like
		fire(1);
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
		back(20);
	}	
}
								