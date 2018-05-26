package aoru;
import robocode.*;
//import java.awt.Color;

// API help : http://robocode.sourceforge.net/docs/robocode/robocode/Robot.html

/**
 * Aoru - a robot by (your name here)
 */
public class Aoru extends AdvancedRobot
{
	/**
	 * run: Aoru's default behavior
	 */
	
	
	public void run() {
		// Initialization of the robot should be put here

		// After trying out your robot, try uncommenting the import at the top,
		// and the next line:

		// setColors(Color.red,Color.blue,Color.green); // body,gun,radar
		setAdjustRadarForRobotTurn(true);
		setAdjustGunForRobotTurn(true);
		
		turnRadarRightRadians(Double.POSITIVE_INFINITY);
		
			
		// Robot main loop
		while(true) {
			// Replace the next 4 lines with any behavior you would like
	
			ahead(0);	
		}
	}

	/**
	 * onScannedRobot: What to do when you see another robot
	 */
	public void onScannedRobot(ScannedRobotEvent e) {
		// Replace the next line with any behavior you would like
		
		setTurnRadarLeftRadians(getRadarTurnRemainingRadians());
		
		double kaku = Math.toRadians(e.getBearing()+getHeading());
		double e_dis = e.getDistance();
		double e_x = e_dis * Math.sin(kaku);
		double e_y = e_dis * Math.cos(kaku);
		
		
				
		if(e_y > 0){
			
	 		turnRight(e.getBearing());
			ahead(e_dis - 100);
		
			if(e_x > 0){
			
				out.println("hidarisita");
				turnLeft(getHeading());
				
				e_dis = e.getDistance();
				e_y = e_dis * Math.cos(kaku);
				
				ahead(e_y);
				setAhead(10000);
				turnRight(10000);
			}
			else if(e_x < 0){
				
				out.println("migisita");
				turnLeft(getHeading());
				
				e_dis = e.getDistance();
				e_y = e_dis * Math.cos(kaku);
				
				ahead(e_y);
				setAhead(10000);
				turnLeft(10000);
			}
		
		}
		else if(e_y < 0){
			
			turnRight(e.getBearing());
			ahead(e_dis - 100);
			
			if(e_x > 0){
				
				out.println("hidariue");
				turnRight(180 - getHeading());
				
				e_dis = e.getDistance();
				e_y = e_dis * Math.cos(kaku);
				
				ahead(-e_y);
				setAhead(10000);
				turnLeft(10000);
			}
			else if(e_x < 0){
				
				out.println("migiue");
				turnLeft(getHeading() - 180);
				
				e_dis = e.getDistance();
				e_y = e_dis * Math.cos(kaku);
				
				ahead(-e_y);
				setAhead(10000);
				turnRight(10000);
			}
		}
			
	
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

		back(100);	
	}	
}
								