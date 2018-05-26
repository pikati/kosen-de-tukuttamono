package adsikaku;
import robocode.*;

//import java.awt.Color;

// API help : http://robocode.sourceforge.net/docs/robocode/robocode/Robot.html

/**
 * Adsikaku - a robot by (your name here)
 */
public class Adsikaku extends AdvancedRobot
{
	/**
	 * run: Adsikaku's default behavior
	 */
		
		
	public void run() {
		// Initialization of the robot should be put here

		// After trying out your robot, try uncommenting the import at the top,
		// and the next line:

		// setColors(Color.red,Color.blue,Color.green); // body,gun,radar

		// Robot main loop
		setAdjustGunForRobotTurn(true);
		setAdjustRadarForRobotTurn(true);
		
		double height = getBattleFieldHeight();
		double width  = getBattleFieldWidth();

		double x = getX();
		double y = getY();
		
		turnLeft(getHeading());
		ahead(height-y);
		turnRight(90);
		ahead(width-x);
		turnRight(90);
		
		setTurnRadarRightRadians(Double.POSITIVE_INFINITY);
	
		
		if (height == 800 && width == 1000)
		{
			ahead(height * 0.15);
			out.println("800 1000");

		//tate to yoko ni ikkaizutsu idou site sumini idou
		
		while(true) {
			// Replace the next 4 lines with any behavior you would like
			ahead(height * 0.7);
			setAhead(200);
			turnRight(90);
			ahead(width * 0.7);
			setAhead(200);
			turnRight(90);
		}
		}
		
		else if(height == 600 && width == 800)
		{
			ahead(height * 0.15);
			out.println("600 800");

		//tate to yoko ni ikkaizutsu idou site sumini idou
		
		while(true) {
			// Replace the next 4 lines with any behavior you would like
			ahead(height * 0.65);
			setAhead(200);
			turnRight(90);
			ahead(width * 0.7);
			setAhead(200);
			turnRight(90);
		}
		}
		
		else if(height == 400 && width == 600)
		{
			ahead(height * 0.15);
			out.println("400 600");

		//tate to yoko ni ikkaizutsu idou site sumini idou
		
		while(true) {
			// Replace the next 4 lines with any behavior you would like
			ahead(height * 0.65);
			out.println("height * 0.65");
			setAhead(200);
			turnRight(90);
			ahead(width * 0.7);
			setAhead(200);
			turnRight(90);
		}
		}
		
		else
		{
			ahead(height * 0.1);
			out.println("seihoukei");

		//tate to yoko ni ikkaizutsu idou site sumini idou
		
		while(true) {
			// Replace the next 4 lines with any behavior you would like
			ahead(height * 0.75);
			setAhead(200);
			turnRight(90);
			ahead(width * 0.75);
			setAhead(200);
			turnRight(90);
		}
		 
		}
		
		
	}

	/**
	 * onScannedRobot: What to do when you see another robot
	 */
	public void onScannedRobot(ScannedRobotEvent e) {
		// Replace the next line with any behavior you would like
	
		 setTurnRadarLeftRadians(getRadarTurnRemainingRadians());
		
		//inyo http://robowiki.net/wiki/One_on_One_Radar
	}

	/**
	 * onHitByBullet: What to do when you're hit by a bullet
	 */
	public void onHitByBullet(HitByBulletEvent e) {
		// Replace the next line with any behavior you would like
		
	}
	
	/**
	 * onHitWall: What to do when you hit a wall
	 */
	public void onHitWall(HitWallEvent e) {
		// Replace the next line with any behavior you would like
		
	}	
}
								