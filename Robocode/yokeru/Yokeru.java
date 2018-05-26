package yokeru;
import robocode.*;
//import java.awt.Color;

// API help : http://robocode.sourceforge.net/docs/robocode/robocode/Robot.html

/**
 * Yokeru - a robot by (your name here)
 */
public class Yokeru extends AdvancedRobot
{
	/**
	 * run: Yokeru's default behavior
	 */
	//double enemyDistance;
	double enemyEnergyBefore;
	double enemyEnergyAfter;
	double Energy = 100;
	double kaku;
	double enemyDistance;
	int count;
	//double enemyX;
	//double enemyY;


	
		
	public void run() {
		// Initialization of the robot should be put here

		// After trying out your robot, try uncommenting the import at the top,
		// and the next line:

		// setColors(Color.red,Color.blue,Color.green); // body,gun,radar

		// Robot main loop
		
		setAdjustGunForRobotTurn(true);
		setAdjustRadarForRobotTurn(true);
	
		setTurnRadarRight(Double.POSITIVE_INFINITY);
		while(true) {
			// Replace the next 4 lines with any behavior you would like
			
			turnRight(0);

		}
	}

	/**
	 * onScannedRobot: What to do when you see another robot
	 */
	public void onScannedRobot(ScannedRobotEvent e) {
		// Replace the next line with any behavior you would like
		setTurnRadarLeft(getRadarTurnRemainingRadians());
		
		
	//	kaku = Math.toRadians(e.getBearing()+getHeading());
		enemyDistance = e.getDistance();
		//enemyX = enemyDistance ＊ Math.sin(kaku);
		//enemyY = enemyDistance ＊ Math.cos(kaku);
		
		
//		if(enemyDistance != 350){
		
//			turnRight(e.getBearing());	
//			ahead(enemyDistance - 350);
//		}
//		out.println(enemyDistance);
//		turnRight(50);
//		turnLeft(50);
//		if (enemyDistance < 200){
			
//			ahead(100);
//		}
		
		turnRight(e.getBearing() - 90);
		
		enemyEnergyBefore = e.getEnergy();
		enemyEnergyAfter = e.getEnergy();
			
		out.println(enemyEnergyBefore + "B");
		out.println(enemyEnergyAfter + "A");
		out.println(Energy +" eeeee");
		out.println(enemyDistance + "nnnn");

		if(enemyEnergyBefore > Energy || enemyEnergyAfter > Energy || enemyEnergyBefore < Energy || enemyEnergyAfter < Energy){
		
			out.println(enemyEnergyBefore + " BBBBB");
			out.println(enemyEnergyAfter +" AAAAA");
			out.println(Energy +" eeeee");
			
			count++;
			
			if(count%2 == 0){
				
				back(60);
				}
//			else if (count % 3 == 0){
		
//				back(30);
	//		}
			
			else {
//				
			ahead(60);
			}
		}
		Energy = enemyEnergyBefore;
		
//   300 ~ 450 kuraide kawaseru

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
		setBack(20);
		turnRight(30);
	}	
}
								