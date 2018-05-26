package neko;
import robocode.*;
import java.awt.Color;

// API help : http://robocode.sourceforge.net/docs/robocode/robocode/Robot.html

/**
 * Neko - a robot by (your name here)
 */
public class Neko extends AdvancedRobot
{
	double radius; //aite no en no nankei
	double kaku;   //iti wo yosou sitaato no kakudo
	double kaku2;	//zahyu wo siraberu tame no kakudo
	double e_X;    //sokutei sitatoki no aite no Xzahyou
	double e_Y;    //aite no Yzahyou
	double e_head; //aite no muiteiru houkou(radian)
	double e_checkTime;		//zikan kankaku wo siraberu tameno zikan
	double e_changeHead;	//kakuhenkaryou
	double e_changeHeadSpeed;	//kakuhenka no supi-do 
	double e_ang;	//aite no yosoku kakudo
	double my_gunHeading;
	double timeInterval;	//zikankankaku
	double bulletSpeed;	//tama no hayasa
	double nextX;	//yosou sita Xzahyou
	double nextY;	//yosou sita Yzahyou 
	double a = 0;	//ikkaime ha nanimo sasenai;
	double rel_X;	//soutai kyori
	double rel_Y;	//soutaikyori
	double tick;	//ima no ge-mu zikan
	double e_energy;	//konkai no aite no energy
	double e_energyBefore;	//zenkai no aite no energy 

	public void run() {
		setColors(Color.yellow,Color.lightGray,Color.yellow);
		setAdjustRadarForRobotTurn(true);
		setAdjustGunForRobotTurn(true);
		setAdjustRadarForGunTurn(true);
		
		setTurnRadarRightRadians(Double.POSITIVE_INFINITY);

		while(true) {
			setTurnRight(50);
			ahead(50);
			setTurnLeft(50);
			ahead(50);
			}
		
	}

	public void onScannedRobot(ScannedRobotEvent e) {
		setTurnRadarLeftRadians(getRadarTurnRemainingRadians());
		
		e_energyBefore = e.getEnergy();
		e_head = e.getHeadingRadians();
		tick = getTime();
		
		if(e.getDistance() < 200){
			setTurnRight(e.getBearing() - 180);
			setAhead(100);
		}

		if(e_energyBefore > e_energy || e_energyBefore < e_energy){
			if(tick%10 == 0){
				setAhead(400);
				turnRight(40);
				turnLeft(100);
			}
			
			else if(tick%10 == 1){
				setTurnRight(100);
				setAhead(120);
			}
			
			else if(tick%10 == 2){
				setTurnRight(60);
				setAhead(120);
			}
			
			else if(tick%10 == 3){
				setTurnRight(40);
				setAhead(120);
			}
			
			else if(tick%10 == 4){
				setTurnRight(50);
				setAhead(100);
			}
			
			else if(tick%10 == 5){
				setTurnRight(70);
				setAhead(130);
			}
			
			else if(tick%10 == 6){
				setTurnRight(80);
				setAhead(140);
			}

			else if(tick%10 == 7){
				setTurnRight(40);
				setAhead(100);
			}
			
			else if(tick%10 == 8){
				setTurnRight(30);
				setAhead(100);
			}
			
			else if(tick%10 == 9){
				setTurnRight(60);
				setAhead(120);
			}
		}
	
		e_energy = e_energyBefore;

		if(a == 0){
		}
		else if(a != 0){
			kaku2 = Math.toRadians(e.getBearing() + getHeading());
			e_X = e.getDistance() * Math.sin(kaku2) + getX();	//aite no Xzahyou
			e_Y = e.getDistance() * Math.cos(kaku2) + getY();	//aite no Yzahyou
			e_head = e.getHeading();							
			e_changeHead = e.getHeadingRadians() - e_head;	//kakuhenkaryou
			timeInterval = getTime() - e_checkTime;			//zikankankaku
			e_changeHeadSpeed = e_changeHead / timeInterval;//kakuhenka no hayasa 
			bulletSpeed = e.getDistance() / 20 - 3 * 3;			//zibun no tama ga ataru made no zikan
			radius = e.getVelocity() / e_changeHeadSpeed;	//aite no hankei
			kaku = e_changeHeadSpeed * bulletSpeed;				
			nextX = e_X + radius * Math.sin(e_head) - Math.sin(e_head + kaku);
			nextY = e_Y + radius * Math.cos(e_head) - Math.cos(e_head + kaku);
			rel_X = nextX - getX();		//soutaikyori
			rel_Y = nextY - getY();		//soutaikyori
			e_ang = Math.atan(rel_Y/rel_X);	//kaiten ni hituyou na kakudo
			my_gunHeading = getGunHeadingRadians();
			
			//gun wo yosokusita iti ni idou saseru
			//aite ga zibunyori migiue
			if(getX() < e_X && getY() < e_Y){
				//gun no muki dai1 syougen
				if(my_gunHeading < Math.PI/2){
					turnGunRightRadians(Math.PI/2 - e_ang - my_gunHeading);
					fire(3);
				}
				//gun no muki dai2 syougen
				else if(my_gunHeading < Math.PI && my_gunHeading > Math.PI/2){
					turnGunLeftRadians(my_gunHeading - Math.PI/2 + e_ang);
					fire(3);
				}
				//gun no muki dai3 syougen
				else if(my_gunHeading < Math.PI/(3.0/2.0) && my_gunHeading > Math.PI){
					turnGunLeftRadians(my_gunHeading - Math.PI/2 + e_ang);
					fire(3);
				}
				//gun no muki dai4 syougen
				else if(my_gunHeading < Math.PI*2 && my_gunHeading > Math.PI/(3.0/2.0)){
					turnGunRightRadians((Math.PI*2 - my_gunHeading) + Math.PI/2 - e_ang);
					fire(3);
				}
			}
			//aite ga zibunyori migisita
			else if(getX() < e_X && getY() > e_Y){
				//gun no muki dai1 syougen
				if(my_gunHeading < Math.PI/2){
					turnGunRightRadians(Math.PI/2 - e_ang - my_gunHeading);
					fire(3);
				}
				//gun no muki dai2 syougen
				else if(my_gunHeading < Math.PI && my_gunHeading > Math.PI/2){
					turnGunLeftRadians(my_gunHeading - Math.PI/2 + e_ang);
					fire(3);
				}
				//gun no muki dai3 syougen
				else if(my_gunHeading < Math.PI * (3.0 / 2.0) && my_gunHeading > Math.PI){
					turnGunLeftRadians(my_gunHeading - Math.PI/2 - e_ang);
					fire(3);
				}
				//gun no muki dai4 syougen
				else if(my_gunHeading < Math.PI*2 && my_gunHeading > Math.PI * (3.0/2.0)){
					turnGunRightRadians((Math.PI * 2 - my_gunHeading) + Math.PI/2 - e_ang);
					fire(2);
				}
			}
			//aite ga zibunyori hidarisita
			else if(getX() > e_X && getY() > e_Y){
				if(my_gunHeading < Math.PI/2){
					//gun no muki dai1 syougn
					turnGunLeftRadians(Math.PI/2 + e_ang + my_gunHeading);
					fire(3);
				}
				//gun no muki dai2 syougen
				else if(my_gunHeading < Math.PI && my_gunHeading > Math.PI/2){
					turnGunRightRadians(Math.PI - my_gunHeading + Math.PI/2 - e_ang);
					fire(3);
				}
				//gun no muki dai3 syougen
				else if(my_gunHeading < Math.PI * (3.0/2.0) && my_gunHeading > Math.PI){
					turnGunRightRadians(Math.PI * (3.0/2.0) - my_gunHeading - e_ang);
					fire(3);
				}
				//gun no muki dai4 syougen
				else if(my_gunHeading < Math.PI*2 && my_gunHeading > Math.PI * (3.0/2.0)){
					turnGunLeftRadians(my_gunHeading - Math.PI * (3.0/2.0) + e_ang);
					fire(3);
				}
			}
			//aite ga zibunyori hidariue
			else if(getX() > e_X && getY() < e_Y){
				//gun no muki dai1 syougen
				if(my_gunHeading < Math.PI/2){
					turnGunLeftRadians(Math.PI/2 + e_ang + my_gunHeading);
					fire(3);
				}
				//gun no muki dai2 syougen
				else if(my_gunHeading < Math.PI && my_gunHeading > Math.PI/2){
					turnGunRightRadians(Math.PI * (3.0/2.0) - my_gunHeading - e_ang);
					fire(3);
				}
				//gun no muki dai3 syougen
				else if(my_gunHeading < Math.PI * (3.0/2.0) && my_gunHeading > Math.PI){
					turnGunRightRadians(Math.PI * (3.0/2.0) - my_gunHeading - e_ang);
					fire(3);
				}
				//gun no muki dai4 syougen
				else if(my_gunHeading < Math.PI*2 && my_gunHeading > Math.PI * (3.0/2.0)){
					turnGunLeftRadians(my_gunHeading - Math.PI * (3.0/2.0) + e_ang);
					fire(3);
				}
			}
		}
		a = 1;
		e_changeHead = 0;
		e_checkTime = getTime() - 1;
		e_head = e.getHeadingRadians();
	}
	
	public void onHitByBullet(HitByBulletEvent e) {
		
	}
	public void onHitWall(HitWallEvent e) {
		setTurnLeft(100);
		back(150);
	}	
}								