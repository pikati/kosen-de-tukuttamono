package pikatan;
import robocode.*;
import java.awt.Color;

/**
 * Pikatan - a robot by (your name here)
 */
public class Pikatan extends AdvancedRobot
{
	double e_Energy = 100;
	double e_EnergyBefore = 0;
	/*double e_EnergyAfter  = 0;*/
	double my_x;
	double my_y;
	double my_gunHead; /*zibun no gun no kaitenryo*/
	double e_x;
	double e_y;
	double e_nextX; /*yosokugo no Xzahyou*/
	double e_nextY; /*yosokugo no Yzahyou*/
	double e_head; //henkamae no aite no tyuusin kara no kakudo
	double e_kaku;  /*zibun karano aite no kakudo(iti)*/
	double rel_x; /*zibun to aite no Xzahyou no sa*/ 
	double rel_y; /*zibun to aite no Yzahyou no sa*/
	double e_vel;
	double e_henkakaku;//zenkai to konkai no aite no kaudo wo kurabete kakuhenkaryo wo motomeru
	double e_movekaku; /*aite no kakuhenkaryo*/
	double e_tick; //tick to kore wo hikakusite idou sita zikan wo motomeru
	double kaku;
	double henka_vel; //kakudohenka no speed
	double radius; //teki no hankei
	double gunTurn; /*aite wo nerautame ni ugokasu gyusin no kakudo*/
	double bulletVel;
	double bulletTime; /*zibun no tama no hayasa*/
	double tick;
	double e_Distance;
	
	public void run() {

		setColors(Color.yellow,Color.lightGray,Color.yellow);
		setAdjustRadarForRobotTurn(true);
		setAdjustGunForRobotTurn(true);
		
		setTurnRadarRightRadians(Double.POSITIVE_INFINITY);
		
		
		while(true) {
			ahead(0);
			}
	}
	public void onScannedRobot(ScannedRobotEvent e) {
		setTurnRadarLeftRadians(getRadarTurnRemainingRadians());
		 
		e_EnergyBefore = e.getEnergy();
		e_head = e.getHeadingRadians();
		tick = getTime();
		
		if(e_EnergyBefore > e_Energy || /*enemyEnergyAfter > Energy ||*/ e_EnergyBefore < e_Energy /*|| enemyEnergyAfter < Energy*/){
		
			out.println(e_EnergyBefore + " BBBBB");
			//out.println(enemyEnergyAfter +" AAAAA");
			out.println(e_Energy +" eeeee");
			
			
			
			if(tick%10 == 0){
				setTurnRight(50);
				setAhead(60);
				out.println(tick +" 0");
				}
			
			else if(tick%10 == 1){
				setTurnLeft(50);
				setAhead(60);
				out.println(tick +" 1");
				}
			
			else if(tick%10 == 2){
				setTurnRight(50);
				setAhead(60);
				out.println(tick +" 2");
				}
			
			else if(tick%10 == 3){
				setTurnLeft(50);
				setAhead(50);
				out.println(tick +" 3");
				}
			
			else if(tick%10 == 4){
				setTurnRight(50);
				setAhead(50);
				out.println(tick +" 4");
				}
			

			else if(tick%10 == 5){
				setTurnLeft(50);
				setAhead(50);
				out.println(tick +" 5");
				}
			

			else if(tick%10 == 6){
				setTurnRight(40);
				setAhead(50);
				out.println(tick +" 6");
				}

			else if(tick%10 == 7){
				setTurnLeft(40);
				setAhead(50);
				out.println(tick +" 7");
				}
			
			else if(tick%10 == 8){
				setTurnRight(30);
				setAhead(50);
				out.println(tick +" 8");
				}
			
			else if(tick%10 == 9){
				setTurnRight(30);
				setAhead(50);
				out.println(tick +" 9");
				}

			}
		e_Energy = e_EnergyBefore;
		
		//kaku = Math.toRadians(e.getBearing()+getHeading());
		e_head = e.getHeading(); //field kara mita aite no buiteru houkou
		e_Distance = e.getDistance(); // aite tono kyori
		kaku = Math.toRadians(e.getBearing()+getHeading());
		e_x = e_Distance * Math.sin(kaku);
		e_y = e_Distance * Math.cos(kaku);
		my_x = getX(); /*zibun no Xzahyou wo motomeru*/
		my_y = getY(); /*zibun no Yzahyou wo motomeru*/
		my_gunHead = getGunHeading(); /*idoumae no zibun no gun no kakudo(muki)*/
		e_vel = e.getVelocity();
		rel_x = e_x - my_x; /*zibun to aite no kyori no sa e_x de yokune*/
		rel_y = e_y - my_y; /*ue ni onazi*/
		


		e_kaku = Math.atan2(e_x, e_y) /*+ Math.PI * 180 / Math.PI*/; //bessi ni kakudo ga aru  (aite no iti)



		out.println(e_kaku * 180 / Math.PI + 180 + "e_kaku"); //sita ga zero onazi houkou ni zouka
		
		

		
		if(e_Distance < 300 || e_Distance > 300/*&& e_Distance >= 200*/){
			//if(my_x > e_x && my_y > e_y){
				e_henkakaku = e.getHeadingRadians() - e_kaku;
				e_tick = getTime() - tick;
				henka_vel = e_henkakaku / e_tick;
				setTurnGunRight(e_head);
				bulletVel = 20 - 3 * 0.1;
				bulletTime = e_Distance / bulletVel;
				e_movekaku = e_vel / bulletTime; //idou sita kakuryou (kaku = kakuhenka no speed * eneagy tama no toutatu zikan)
				radius = e_vel / henka_vel;
				e_nextX = e_x + radius * Math.cos(e_head) - radius * Math.cos(e_head + e_movekaku);
				e_nextY = e_y + radius * Math.sin(e_head) - radius * Math.sin(e_head + e_movekaku);
				gunTurn = Math.atan2(e_nextX, e_nextY) * 180 / Math.PI;
				out.println(gunTurn + "turngun");
				out.println(Math.atan2(e_nextX, e_nextY) + "turngun(rad)");
				if(getHeading() > gunTurn){
					turnGunRight(180 - getHeading() + gunTurn);
					fire(0.1);
					out.println("migi");
					}
				else{
					turnGunLeft(gunTurn);
					fire(0.1);
					out.println("hidari");
				}
			//}
		}
	/*	if(e_Distance < 300 && e_Distance >= 200){
			turnGunRight(e.getBearing());
			fire(0.1);
			out.println("0.1");
		}
		
		else if(e_Distance < 200 && e_Distance >= 100){
			turnGunRight(e.getBearing());
			fire(0.5);
			out.println("0.5");
		}
		
		else if(e_Distance < 100 && e_Distance >= 50){
			turnGunRight(e.getBearing());
			fire(1.0);
			out.println("1.0");
		}
		
		else if(e_Distance < 50){
			turnGunRight(e.getBearing());
			fire(3.0);
			out.println("3.0");
		}
		
		else{
			setTurnRight(e.getBearing());
			setAhead(100);
		}*/
	}

	public void onHitByBullet(HitByBulletEvent e) {
		setColors(Color.red,Color.red,Color.red);

		setTurnRight(50);
		setBack(50);
	}

	public void onHitWall(HitWallEvent e) {
		setBack(20);
	}	
	
	public void onWin(WinEvent e){
		setColors(Color.yellow,Color.yellow,Color.yellow);
	}
}
								