#include<stdio.h>
#include <stdbool.h>

/*
	Algoritma i�erisinde baz� yorumlar �ok anla��l�r olmayabilir.
	�rne�in ileride k��e diye bir terimden bahsedilmekte.
	Burada bahsedilen geometrik �ekillerden bildi�imiz k��e ile ayn� de�il.
	Buradaki k��enin tan�m� bir karenin 2'den fazla yol ba�lant�s�na sahip olmas�d�r.
	K��e denilmesinin nedeni algoritma i�erisindeki en k�sa yolu hesaplama b�l�m� ile alakal�.
	Bu gibi anlam� biraz kapal� olan k�s�mlar hakk�nda a��klama dosyas� yaz�lmas� planlanmakta.
*/

/* 12x12 olmak �zere 144 kareden olu�an bir labirent i�in yaz�yorum kodu.
   Algoritma i�leyi�i:
   Robot her karede sa�,sol ve �ndeki sens�rlerden gelen veri ile yol a��k m� yoksa duvar m� var bunu alg�lar.
   E�er sadece bir taraf a��ksa o karede zaten tek yol vard�r ve ordan gitmek zorunludur. E�er birden fazla taraf a��ksa
   o kare i�in belli karar mekanizmalar� kullanmak gerekiyor.
   Karelerin bilgilerini tutacak de�i�kenleri i�eren bir yap� tan�mlay�p o tipte 2 boyutlu bir dizi tan�mlanacak.
   Bu dizi kare bilgilerini bellekte tutmak ve gerekli i�lemleri yapmak i�in kullan�lacak.
   
   Labirentin x-y d�zleminde oldu�unu varsay�yorum ve robotun ba�lang�� noktas�n� (0,0) kabul ediyorum. Robotun
   ba�lang��ta bakt��� y�n, +y y�n� kabul ediliyor. Bu y�n ve konum bilgileri kullan�larak robotu hareket ettirme
   ve konumunu belirleme i�lemleri yap�lacak.
*/

/*
 Bu maze yap�s� labirent haritas�n� tutacak olan dizi i�in olu�turuldu. Burada bir nevi sim�lasyon program� yaz�ld��� i�in
 labirent haritas�n� programa elle girmek gerekiyor. Kodu robota uyarlarken bu yap� ve dizi kald�r�lacak.
 �rne�in ger�ekte ileri yol a��k m� bilgisi �ndeki sens�rden gelen veri ile anla��lacak. Burada ise tan�mlanacak olan maze
 dizisinin ilgili nesnesi kontrol edilerek anla��lacak. Yap� i�inde 5 tane nesne olacak +y -y +x -x ve hedef. Bu nesneler ilgili karenin
 o y�nlerinde duvar var m� yok mu bilgisini tutuyor olacak ve hedef o karenin ula��lmak istenen kare olup olmad��� bilgisini tutacak.
 A��kcas� labirent bilgilerini girmek uzun ve zahmetli bir i� oldu�u i�in program her ba�lad���nda kullan�c�n�n bunu yapmas�
 ho� olmaz. O y�zden burada maze isimli bir labirent giricem. Labirent de�i�tirilmek istenirse kodun i�inde buras� de�i�tirilmeli. 
*/

char maze[25][25] = {"ooooooooooooooooooooooooo",
					 "o                       o",
					 "ooooo o ooooooooo ooooo o",
					 "o o   o o     o   o   o o",
					 "o o o o o ooo o ooo ooo o",
					 "o   o o     o o     o   o",
					 "o ooo ooo ooo o ooo ooo o",
					 "o o o o     o o   o o   o",
					 "ooo o o ooooooooo o ooo o",
					 "o o   o           o o o o",
					 "o ooo ooooooooooooo o o o",
					 "o     o   ox x        o o",
					 "o ooooo ooo   ooooooo o o",
					 "o   o   o ox xo o o   o o",
					 "o o ooo o ooooo o ooo o o",
					 "o o o o           o o o o",
					 "ooo o ooooo ooooooo ooo o",
					 "o   o o       o         o",
					 "o ooo o o ooo o o ooooooo",
					 "o o     o   o o o o   o o",
					 "o ooooo ooooooo ooo o o o",
					 "o o o o   o         o o o",
					 "o o o ooo o ooo ooooo o o",
					 "o             o     o   o",
					 "ooooooooooooooooooooooooo"};
// robot gezerken ziyaret etti�i kareleri i�aretleme,
// ��kmaza giden yollar� kapatma gibi i�lemler yap�yor.
// bunlar� python sim�lasyonunda g�rmek i�in bu ikinci maze 
// dizisini kullan�yoruz.
char maze2[25][25] = {"ooooooooooooooooooooooooo",
					  "o                       o",
					  "ooooo o ooooooooo ooooo o",
					  "o o   o o     o   o   o o",
					  "o o o o o ooo o ooo ooo o",
					  "o   o o     o o     o   o",
					  "o ooo ooo ooo o ooo ooo o",
					  "o o o o     o o   o o   o",
					  "ooo o o ooooooooo o ooo o",
					  "o o   o           o o o o",
					  "o ooo ooooooooooooo o o o",
					  "o     o   ox x        o o",
					  "o ooooo ooo   ooooooo o o",
					  "o   o   o ox xo o o   o o",
					  "o o ooo o ooooo o ooo o o",
					  "o o o o           o o o o",
					  "ooo o ooooo ooooooo ooo o",
					  "o   o o       o         o",
					  "o ooo o o ooo o o ooooooo",
					  "o o     o   o o o o   o o",
					  "o ooooo ooooooo ooo o o o",
					  "o o o o   o         o o o",
					  "o o o ooo o ooo ooooo o o",
					  "o             o     o   o",
					  "ooooooooooooooooooooooooo"};


// Karelerin bilgilerini tutan yap�.
struct square_inf{
	int visit; // Kareye daha �nce ka� kere gelindi�ini tutan de�i�ken.
	int y_direction; // Herhangi bir karenin 4 taraf�ndaki ge�it i�in a��k m� yoksa kapal� m� oldu�unu tutan de�i�kenler.
	int v_direction; // 1 a��k, 0 kapal�, 2 g�r�lmedi demek.
	int x_direction; // Labirenti 2 boyutlu x-y eksenine oturtuyoruz ve y�nleri �u �ekilde kodluyoruz.
	int c_direction; // +y y�n� i�in y, -y i�in v, +x i�in x, -x i�in c kullan�yoruz.
	bool finish; // O karenin ula��lmak istenen kare olup olmad���n� tutan de�i�ken. 1 evet 0 hay�r.
	
	
};
struct square_inf square[12][12];

// Robot bilgilerini tutan yap�
struct robot_inf{
	// y�n ve konum de�i�kenleri.
	// y�n i�in +y = y, -y = v, +x = x, -x = c olarak kullan�lacak.
	// konum square dizisine g�re bulundu�umuz karenin s�tun ve sat�r numaralar�.
	char direction;
	int column; // robotun labirentteki konumu. Sutun ve sat�r olarak.
	int line;
	int front; // robotun �n sa� ve solundaki ge�itlerin a��k m� kapal� m� oldu�unu tutan de�i�kenler
	int right;
	int left;
	int front_visit; // robotun �n sa� ve solundaki karelerin ziyaret edilme say�lar�n� tutan de�i�kenler
	int right_visit;
	int left_visit;
};
struct robot_inf robot;

// En k�sa yolu bulmaya �al���rken yapaca��m�z i�lemler i�in k��elerin bilgilerini tutacak olan yap�.
struct vertice_inf{
	int column;
	int line;
	int number_of_connections; // ka� farkl� k��eye ba�lan�yor.
	int connection[4]; // hangi k��elere ba�land���n� tuttu�umuz de�i�kenler.
                       // K��elerin isimleri = vertice dizisindeki indexleri.
                       // connection[0] y y�n�, connection[1] v y�n�, connection[2] x y�n�, connection[3] c y�n�.
	int path_distance[4]; // ba�lant� yollar�n�n uzunluklar�. connection dizisi ile ayn� indexli
	int checked; // en k�sa yolu hesaplarken o k��eye u�ray�p u�ramad���m�z� kontrol etmek i�in bu de�i�keni kontrol edicez. 
};
struct vertice_inf vertice[50]; // ka� tane k��e olaca��n� bilemiyorum. 50'den fazla olmaz diye d���n�yorum. 50 bile baya abart�.
int number_of_vertices; // k��e say�s�
int path[50];
int shortest_path[50];
int distance,shortest_distance;

void find_the_target_section();
void descripe_square();
void descripe_robot();
void choose_and_turn();
void turn_right();
void turn_left();
void turn_around();
void go_straight();

void find_the_shortest_path();
int check_y(int column, int line, int vertice_index, int connection_index, int path_distance);
int check_v(int column, int line, int vertice_index, int connection_index, int path_distance);
int check_x(int column, int line, int vertice_index, int connection_index, int path_distance);
int check_c(int column, int line, int vertice_index, int connection_index, int path_distance);
int path_comparator(int v_index, int path_index);

void go_back_to_first_section();
void go_to_target_section();
void write();
void write2();
void write3();

int hamle;

int main(){
	// Python �zerinden sim�lasyon yapmak i�in verileri dosyalara yaz�yoruz.
	// Sonra o verileri pythonda dosyadan okuyarak sim�lasyon yap�yoruz.
	// Kodu robota aktar�rken dosya i�lemleri silinecek.
	FILE *filep;
	filep = fopen("maze runner.txt","w");
	FILE *filep2;
	filep2 = fopen("maze.txt","w");

	find_the_target_section();
	find_the_shortest_path();
	go_back_to_first_section();
	
	
	go_to_target_section();
	go_back_to_first_section();
	go_to_target_section();
	go_back_to_first_section();
	

	return 0;
}

void find_the_target_section(){
	// Ba�lang�� i�in bu y�n� ve konumu kabul ediyoruz.
	// Robotun labirente nas�l koyuldu�unun �nemi yok. 4 k��eden biri olmas� yeterli.
	// Ancak e�er k��e de�ilse kodda biraz oynama yapmak gerek ��nk� C'de arraylere negatif
	// index verilmiyor.
	robot.direction = 'y';
	robot.column = 0;
	robot.line = 0;
	robot.front_visit = 0;
	robot.right_visit = 0;
	robot.left_visit = 0;
	
	// ba�lang��ta t�m kareleri ziyaret edilmedi ve biti� de�il i�aretliyoruz.
	// Ve t�m karelerin 4 taraf�ndaki ge�itleri g�r�lmedi olarak i�aretliyoruz. 
	int i,a; 
	for(i=0; i<=11; i++){
		for(a=0; a<=11; a++){
			square[i][a].visit=0;
			square[i][a].finish=0;
			square[i][a].y_direction = 2; // 1 a��k, 0 kapal�, 2 g�r�lmedi demek.
			square[i][a].c_direction = 2;
			square[i][a].v_direction = 2;
			square[i][a].x_direction = 2;
		}
	}
	
	
	// Hedef kareyi bulana kadar d�necek olan d�ng�
	while(1){
		descripe_square();
		descripe_robot();
		
		if(square[robot.column][robot.line].finish == 1){ // e�er hedefe ula�t�ysak d�ng�y� bitiriyoruz.
			break;
		}
		
		choose_and_turn();
		hamle++;
		write2(); // G�ncel labirent bilgilerini dosyaya yaz�yoruz. 
		go_straight();
		write(); // Hamleleri ekrana ve dosyaya yaz�yoruz.
	}
}

void descripe_square(){
	// burada normalde sens�rlerden gelen verilere g�re i�lem yap�caz.
	// as�l konumun maze hali: square +1 +1
	int column_maze = robot.column*2+1;
	int line_maze = robot.line*2+1;
	if(square[robot.column][robot.line].visit == 0){
		// daha �nce ziyaret edilmedi ise 1 kere ziyaret edildi olarak i�aretliyoruz 
		// ve karenin 4 taraf�ndaki ge�itleri kontrol ediyoruz a��k m� kapal� m� diye.
		square[robot.column][robot.line].visit += 1;
		if(maze[column_maze][line_maze] == 'x'){
			square[robot.column][robot.line].finish = 1; // e�er x ise oras� hedef karedir.
		}
		// bakt��� y�ne g�re, maze dizisindeki kontrol edece�i eleman de�i�iyor
		// o y�zden her y�n i�in ayr� ayr� yaz�yoruz.
		if(robot.direction == 'y'){
			// arka zaten a��kt�r.
			square[robot.column][robot.line].v_direction = 1;
			//�n i�in: maze -1 0
			if(maze[column_maze-1][line_maze] == 'o'){
				square[robot.column][robot.line].y_direction = 0;
			}
			else if(maze[column_maze-1][line_maze] == ' ' || maze[column_maze-1][line_maze] == 'x'){
				square[robot.column][robot.line].y_direction = 1;
			}
			//sa� i�in: maze 0 +1
			if(maze[column_maze][line_maze+1] == 'o'){
				square[robot.column][robot.line].x_direction = 0;
			}
			else if(maze[column_maze][line_maze+1] == ' ' || maze[column_maze][line_maze+1] == 'x'){
				square[robot.column][robot.line].x_direction = 1;
			}
			//sol i�in: maze 0 -1
			if(maze[column_maze][line_maze-1] == 'o'){
				square[robot.column][robot.line].c_direction = 0;
			}
			else if(maze[column_maze][line_maze-1] == ' ' || maze[column_maze][line_maze-1] == 'x'){
				square[robot.column][robot.line].c_direction = 1;
			}
		}
		else if(robot.direction == 'v'){
			// arka zaten a��kt�r.
			square[robot.column][robot.line].y_direction = 1;
			//�n i�in: maze +1 0
			if(maze[column_maze+1][line_maze] == 'o'){
				square[robot.column][robot.line].v_direction = 0;
			}
			else if(maze[column_maze+1][line_maze] == ' ' || maze[column_maze+1][line_maze] == 'x'){
				square[robot.column][robot.line].v_direction = 1;
			}
			//sa� i�in: maze 0 -1
			if(maze[column_maze][line_maze-1] == 'o'){
				square[robot.column][robot.line].c_direction = 0;
			}
			else if(maze[column_maze][line_maze-1] == ' ' || maze[column_maze][line_maze-1] == 'x'){
				square[robot.column][robot.line].c_direction = 1;
			}
			//sol i�in: maze 0 +1
			if(maze[column_maze][line_maze+1] == 'o'){
				square[robot.column][robot.line].x_direction = 0;
			}
			else if(maze[column_maze][line_maze+1] == ' ' || maze[column_maze][line_maze+1] == 'x'){
				square[robot.column][robot.line].x_direction = 1;
			}
		}
		else if(robot.direction == 'x'){
			// arkda zaten a��kt�r.
			square[robot.column][robot.line].c_direction = 1;
			//�n i�in: maze 0 +1
			if(maze[column_maze][line_maze+1] == 'o'){
				square[robot.column][robot.line].x_direction = 0;
			}
			else if(maze[column_maze][line_maze+1] == ' ' || maze[column_maze][line_maze+1] == 'x'){
				square[robot.column][robot.line].x_direction = 1;
			}
			//sa� i�in: maze +1 0
			if(maze[column_maze+1][line_maze] == 'o'){
				square[robot.column][robot.line].v_direction = 0;
			}
			else if(maze[column_maze+1][line_maze] == ' ' || maze[column_maze+1][line_maze] == 'x'){
				square[robot.column][robot.line].v_direction = 1;
			}
			//sol i�in: maze -1 0
			if(maze[column_maze-1][line_maze] == 'o'){
				square[robot.column][robot.line].y_direction = 0;
			}
			else if(maze[column_maze-1][line_maze] == ' ' || maze[column_maze-1][line_maze] == 'x'){
				square[robot.column][robot.line].y_direction = 1;
			}
		}
		else if(robot.direction == 'c'){
			// arka zaten a��kt�r.
			square[robot.column][robot.line].x_direction = 1;
			//�n i�in: maze 0 -1
			if(maze[column_maze][line_maze-1] == 'o'){
				square[robot.column][robot.line].c_direction = 0;
			}
			else if(maze[column_maze][line_maze-1] == ' ' || maze[column_maze][line_maze-1] == 'x'){
				square[robot.column][robot.line].c_direction = 1;
			}
			//sa� i�in: maze -1 0
			if(maze[column_maze-1][line_maze] == 'o'){
				square[robot.column][robot.line].y_direction = 0;
			}
			else if(maze[column_maze-1][line_maze] == ' ' || maze[column_maze-1][line_maze] == 'x'){
				square[robot.column][robot.line].y_direction = 1;
			}
			//sol i�in: maze +1 0
			if(maze[column_maze+1][line_maze] == 'o'){
				square[robot.column][robot.line].v_direction = 0;
			}
			else if(maze[column_maze+1][line_maze] == ' ' || maze[column_maze+1][line_maze] == 'x'){
				square[robot.column][robot.line].v_direction = 1;
			}
		}
	}
	else{
		// E�er daha �nce ziyaret edildi ise tekrar 4 taraf� kontrol etmeye gerek yok.
		// Visit de�i�kenini 1 artt�r�yoruz.
		// ��kmaz yoldan geliyor olma ihtimalimiz var. O y�zden bir �nceki karenin ��kmaz olup olmad���n�
		// kontrol edicez. E�er ��kmazsa yani 3 taraf� kapal� ise geldi�imiz ge�iti de kapal� i�aretlicez.
		// B�ylece robot oraya tekrar gelirse oray� kapal� g�recek ve girmicek. ��kmazdan d�nerken
		// bu i�lemi yapmaya devam etti�imiz i�in ��kmaza giden t�m yollar kapal� i�aretlenmi� olacak.
		square[robot.column][robot.line].visit += 1;
		if(robot.direction == 'y'){
			 // bir �nceki karenin ��kmaz olup olmad���n� kontrol ediyoruz.
			if(square[robot.column+1][robot.line].v_direction == 0 && square[robot.column+1][robot.line].x_direction == 0 && square[robot.column+1][robot.line].c_direction == 0){
				// e�er �yleyse ve o kare ba�lang�� karesi de�ilse geldi�imiz ge�iti kapal� olarak i�aretliyoruz.
				if(robot.column+1 != 0 || robot.line != 0){
					square[robot.column][robot.line].v_direction = 0;
				}
			}
		}
		else if(robot.direction == 'v'){
			 // bir �nceki karenin ��kmaz olup olmad���n� kontrol ediyoruz.
			if(square[robot.column-1][robot.line].y_direction == 0 && square[robot.column-1][robot.line].x_direction == 0 && square[robot.column-1][robot.line].c_direction == 0){
				// e�er �yleyse ve o kare ba�lang�� karesi de�ilse geldi�imiz ge�iti kapal� olarak i�aretliyoruz.
				if(robot.column-1 != 0 || robot.line != 0){
					square[robot.column][robot.line].y_direction = 0;
				}
			}
		}
		else if(robot.direction == 'x'){
			 // bir �nceki karenin ��kmaz olup olmad���n� kontrol ediyoruz.
			if(square[robot.column][robot.line-1].c_direction == 0 && square[robot.column][robot.line-1].y_direction == 0 && square[robot.column][robot.line-1].v_direction == 0){
				// e�er �yleyse ve o kare ba�lang�� karesi de�ilse geldi�imiz ge�iti kapal� olarak i�aretliyoruz.
				if(robot.column != 0 || robot.line-1 != 0){
					square[robot.column][robot.line].c_direction = 0;
				}
			}
		}
		else if(robot.direction == 'c'){
			 // bir �nceki karenin ��kmaz olup olmad���n� kontrol ediyoruz.
			if(square[robot.column][robot.line+1].x_direction == 0 && square[robot.column][robot.line+1].y_direction == 0 && square[robot.column][robot.line+1].v_direction == 0){
				// e�er �yleyse ve o kare ba�lang�� karesi de�ilse geldi�imiz ge�iti kapal� olarak i�aretliyoruz.
				if(robot.column != 0 || robot.line+1 != 0){
					square[robot.column][robot.line].x_direction = 0;
				}
			}
		}
	}
}
void descripe_robot(){
	// Kare bilgilerini robotun o anki y�n�ne g�re robot de�i�kenlerine aktar�yoruz.
	// B�ylece karar verme fonksiyonumuzda robotun y�n�n� d���nmemize gerek kalm�yor.
	if(robot.direction == 'y'){
		robot.front = square[robot.column][robot.line].y_direction;
		robot.right = square[robot.column][robot.line].x_direction;
		robot.left = square[robot.column][robot.line].c_direction;
		robot.front_visit = square[robot.column-1][robot.line].visit;
		robot.right_visit = square[robot.column][robot.line+1].visit;
		robot.left_visit = square[robot.column][robot.line-1].visit;
	}
	else if(robot.direction == 'v'){
		robot.front = square[robot.column][robot.line].v_direction;
		robot.right = square[robot.column][robot.line].c_direction;
		robot.left = square[robot.column][robot.line].x_direction;
		robot.front_visit = square[robot.column+1][robot.line].visit;
		robot.right_visit = square[robot.column][robot.line-1].visit;
		robot.left_visit = square[robot.column][robot.line+1].visit;
	}
	else if(robot.direction == 'x'){
		robot.front = square[robot.column][robot.line].x_direction;
		robot.right = square[robot.column][robot.line].v_direction;
		robot.left = square[robot.column][robot.line].y_direction;
		robot.front_visit = square[robot.column][robot.line+1].visit;
		robot.right_visit = square[robot.column+1][robot.line].visit;
		robot.left_visit = square[robot.column-1][robot.line].visit;
	}
	else if(robot.direction == 'c'){
		robot.front = square[robot.column][robot.line].c_direction;
		robot.right = square[robot.column][robot.line].y_direction;
		robot.left = square[robot.column][robot.line].v_direction;
		robot.front_visit = square[robot.column][robot.line-1].visit;
		robot.right_visit = square[robot.column-1][robot.line].visit;
		robot.left_visit = square[robot.column+1][robot.line].visit;
	}
}
void choose_and_turn(){
	// ge�itlerin a��kl�k durumuna g�re hangi y�ne gidece�imizi se�ece�iz.
	// Tek ge�it a��ksa ordan gidece�iz. Birden fazla ise �ncelik s�ras�na g�re se�ece�iz.
	if(robot.front == 0 && robot.right == 0 && robot.left == 0){
		turn_around();
	}
	else if(robot.front == 0 && robot.right == 0){
		//tek d�n�� var
		turn_left();
	}
	else if(robot.front == 0 && robot.left == 0){
		//tek d�n�� var
		turn_right();
	}
	else if(robot.right == 0 && robot.left == 0){
		//sadece �n a��k o y�zden d�nm�yoruz
	}
	// Bundan sonraki ihtimallerde iki tane a��k ge�it var.
	// Bu iki karenin ziyaret edilme say�s�n� kontrol etmeliyiz.
	// Ziyaret edilme say�s� daha az olan kareye gidece�iz.
	else if(robot.front == 0){
		// sa� ve sol a��k.
		if(robot.right_visit <= robot.left_visit){
			turn_right();
		}
		else{
			turn_left();
		}
	}
	else if(robot.right == 0){
		//�n ve sol a��k
		if(robot.front_visit <= robot.left_visit){
			//d�nm�yoruz
		}
		else{
			turn_left();
		}
	}
	else if(robot.left == 0){
		//�n ve sa� a��k
		if(robot.front_visit <= robot.right_visit){
			//d�nm�yoruz
		}
		else{
			turn_right();
		}
	}
	else{
		//t�m y�nler a��k
		if(robot.front_visit <= robot.right_visit && robot.front_visit <= robot.left_visit){
			// d�nm�yoruz
		}
		else if(robot.right_visit <= robot.front_visit && robot.right_visit <= robot.left_visit){
			turn_right();
		}
		else{
			turn_left();
		}
	}
}

void turn_right(){
	if(robot.direction == 'y'){
		robot.direction = 'x';
	}
	else if(robot.direction == 'x'){
		robot.direction = 'v';
	}
	else if(robot.direction == 'v'){
		robot.direction = 'c';
	}
	else if(robot.direction == 'c'){
		robot.direction = 'y';
	}
}
void turn_left(){
	if(robot.direction == 'y'){
		robot.direction = 'c';
	}
	else if(robot.direction == 'x'){
		robot.direction = 'y';
	}
	else if(robot.direction == 'v'){
		robot.direction = 'x';
	}
	else if(robot.direction == 'c'){
		robot.direction = 'v';
	}
}
void turn_around(){
	if(robot.direction == 'y'){
		robot.direction = 'v';
	}
	else if(robot.direction == 'x'){
		robot.direction = 'c';
	}
	else if(robot.direction == 'v'){
		robot.direction = 'y';
	}
	else if(robot.direction == 'c'){
		robot.direction = 'x';
	}
	
}
void go_straight(){
	if(robot.direction == 'y'){
		robot.column -= 1;
	}
	else if(robot.direction == 'x'){
		 robot.line += 1;
	}
	else if(robot.direction == 'v'){
		robot.column += 1;
	}
	else if(robot.direction == 'c'){
		robot.line -= 1;
	}
}
void find_the_shortest_path(){
	// Bu fonksiyon robot hedef kareyi bulduktan sonra �al��acak.
	// Ba�lang�� ile biti� aras�ndaki en k�sa yolu bulucaz.
	// Bunu yaparken ziyaret edilmi� olan karelere g�re i�lem yapaca��z.
	// Ziyaret edilmeyen karelere giden ge�itleri kapal� kabul edece�iz.
	// En k�sa yol ziyaret edilmeyen karelerden ge�iyor da olabilir ancak
	// biz bunu bu a�amada bilemiyoruz. �lerleyen zamanlarda bunun i�in de bir
	// ��z�m �retmeliyiz. �imdilik bu kodu tamamlay�p elimizde �al��an bir kod olmas�
	// �zerine yo�unla��yorum.
	
	// Ziyaret edilmeyen karelere giden ge�itleri kapal� olarak i�aretliyoruz.
	int i=0,a=0,i2=0,a2=0;
	for (i=0; i<=11; i++){
		for(a=0; a<=11; a++){
			if(square[i][a].visit == 0){
				square[i][a].y_direction = 0;
				square[i][a].v_direction = 0;
				square[i][a].x_direction = 0;
				square[i][a].c_direction = 0;
			}
		}
	}
	hamle++;
	//sim�lasyonde g�rmek i�in yazd�r�yoruz
	write3();
	
	// Program bir ge�iti iki farkl� de�i�kende tutuyor. �rne�in 0 0 karesinin x y�n�ndeki ge�iti
	// square[0][0].x_direction de�i�keni ile square[0][1].c_direction de�i�kenleri tutuyor. ��kmaz
	// yola giden ge�itleri kapal� kabul ederken herhangi bir ge�it i�in bu iki de�i�kenden bir tanesini
	// kapal� olarak kabul ettik. Ayn�s�n� ziyaret edilmeyen karelere giden ge�itleri kapal� olarak
	// i�aretlerken de yapt�k. �imdi burda her ge�it i�in o iki de�i�keni birbirine e�itleyece�iz.
	// Sadece biri kapal� ise di�erini de kapal� yap�caz. �kisi de kapal� veya a��ksa de�i�iklik olmicak.	
	for (i=0; i<=10; i++){
		for(a=0; a<=10; a++){ 
			if(square[i][a].x_direction == 0 || square[i][a+1].c_direction == 0){
				square[i][a+1].c_direction = 0;
				square[i][a].x_direction = 0;
			}
			if(square[i][a].v_direction == 0 || square[i+1][a].y_direction == 0){
				square[i+1][a].y_direction = 0;
				square[i][a].v_direction = 0;
			}
		}
	}
	hamle++;
	//sim�lasyonde g�rmek i�in yazd�r�yoruz
	write3();
	
	// Dijkstra en k�sa yol algoritmas�n� yapmaya �al���yorum. Ba�lang��dan hedefe ula�ana kadar yapt���m
	// haritaland�rma sonucunda 3 veya 4 taraf� a��k kalan kareler K��e(Vertices) olacak. 
	// Ba�lang�� ve hedef karelerinin  ka� taraf� a��k oldu�unun �nemi yok onlar da k��e olacak.
	// 2 taraf� a��k olan kareler k��eleri ba�layan yollard�r. �ki k��e aras�ndaki karelerin say�s� yol uzunlu�udur.
	// Labirentteki hedef karesini ba�lang�� k��esi, ba�lang�� karesini de biti� k��esi kabul ederek djikstra algoritmas�n�
	// uygulamaya �al��aca��m.
	
	// ba�lang�� karesi ilk k��emiz.
	vertice[0].column = 0;
	vertice[0].line = 0;
	int wall[4]; // vertice[0]'�n ba�lant� say�s� kontrol edilmeye ba�lan�yor
	wall[0] = square[0][0].y_direction;
	wall[1] = square[0][0].v_direction;
	wall[2] = square[0][0].x_direction;
	wall[3] = square[0][0].c_direction;
	vertice[0].number_of_connections = 0;
	
	for(i=0; i<=3; i++){
		if(wall[i] == 1){
			vertice[0].number_of_connections++;
		}
	}
	
	// Hedef kare ikinci k��emiz. hedef kareyi buluyoruz
	for(i=0; i<=11; i++){
		for(a=0; a<=11; a++){
			if(square[i][a].finish == 1){
				vertice[1].column = i;
				vertice[1].line = a;
			}
		}
	} // vertice [1]'in ba�lant� say�s�n� kontrol etmeye ba�l�yoruz
	wall[0] = square[vertice[1].column][vertice[1].line].y_direction;
	wall[1] = square[vertice[1].column][vertice[1].line].v_direction;
	wall[2] = square[vertice[1].column][vertice[1].line].x_direction;
	wall[3] = square[vertice[1].column][vertice[1].line].c_direction;
	vertice[1].number_of_connections = 0;
	
	for(i=0; i<=3; i++){
		if(wall[i] == 1){
			vertice[1].number_of_connections++;
		}
	}
	
	number_of_vertices = 2; // �u ana kadar iki tane k��e tan�mlad�k.
	
	// di�er k��eleri tan�ml�yoruz. T�m kareleri tar�yorum bunlardan 3 veya 4 taraf� a��k olan kare k��edir.
	for(i=0; i<=11; i++){
		for(a=0; a<=11; a++){
			if((i == 0 && a == 0) || square[i][a].finish == 1){
				// bunlar� zaten tan�mlad�k o y�zden bunlar i�in i�lem yapm�yoruz.
			}
			else{
				wall[0] = square[i][a].y_direction;
				wall[1] = square[i][a].v_direction;
				wall[2] = square[i][a].x_direction;
				wall[3] = square[i][a].c_direction;
				for(i2=0; i2<=3; i2++){
					if(wall[i2] == 1){
						a2++;
					}
				}
				
				if(a2 == 3){
					// 3 ge�it a��k yani bu kare k��e.
					number_of_vertices++;
					vertice[number_of_vertices-1].column = i;
					vertice[number_of_vertices-1].line = a;
					vertice[number_of_vertices-1].number_of_connections = 3;
				}
				else if(a2 == 4){
					// 4 ge�it a��k yani bu kare k��e.
					number_of_vertices++;
					vertice[number_of_vertices-1].column = i;
					vertice[number_of_vertices-1].line = a;
					vertice[number_of_vertices-1].number_of_connections = 4;
				}
				a2 = 0; // kontrol sayac�m�z� s�f�rl�yoruz.
			}
		}
	}
	
	// k��eleri ekrana yaz�yoruz
	
	printf("\n Kose sayisi: %d\n",number_of_vertices);
	for(i=0; i<number_of_vertices; i++){
		printf("Kose %d: %d %d. Baglanti sayisi: %d\n",i,vertice[i].column,vertice[i].line,vertice[i].number_of_connections);
	}
	
	// ba�lant�lar� ve yol uzunluklar�n� tan�ml�yoruz.
	// burada path_distance dizisinin baz� elemanlar� 0 kalacak.
	// oralardan ba�lant� olmad��� anlam�na geliyor.
	// connection dizisinin de ba�lant� olmayan elemanlar� ve vertice[0]'a ba�lanan
	// elemanlar� 0 olacak. o y�zden connection[x] s�f�rsa onun vertice[0]' ba�l� olup olmad���n�
	// anlamak i�in path_distance[x]'i de kontrol etmeliyiz. E�er path_distance[x] s�f�r de�ilse o ba�lant� vard�r.
	for(i=0; i<number_of_vertices; i++){
		for(a=0; a<4; a++){
			vertice[i].path_distance[a] = 0; // ba�lamadan �nce t�m yol uzunluklar�n� ve ba�lant�lar� s�f�r yap�yoruz.
			vertice[i].connection[a] = 0;
		}
	}
	
	for(i=0; i<number_of_vertices; i++){
		//bu 4 fonksiyon o k��enin o y�n�ndeki ge�itten hangi k��eye ba�land���n�
		//ve bu ba�lant�n�n yol uzunlu�unun ne kadar oldu�unu buluyor.
		//her y�n i�in farkl� bir fonksiyon. 
		check_y(vertice[i].column, vertice[i].line, i, 0, 0); 
		check_v(vertice[i].column, vertice[i].line, i, 1, 0);
		check_x(vertice[i].column, vertice[i].line, i, 2, 0);
		check_c(vertice[i].column, vertice[i].line, i, 3, 0);
	}
	// ba�lant�lar� ve yol uzunluklar�n� ekrana yaz�yoruz.
	
	for(i=0; i<number_of_vertices; i++){
		printf("\nKose %d baglantilar:\n",i);
		wall[0]=square[vertice[i].column][vertice[i].line].y_direction;
		wall[1]=square[vertice[i].column][vertice[i].line].v_direction;
		wall[2]=square[vertice[i].column][vertice[i].line].x_direction;
		wall[3]=square[vertice[i].column][vertice[i].line].c_direction;
		for(a=0; a<4; a++){
		//	if(wall[a] == 1){
				printf("Kose %d, Yol uzunlugu %d\n", vertice[i].connection[a], vertice[i].path_distance[a]);
		//	}
		}
		printf("\n");
	}
	
	// Art�k t�m k��eleri, k��elerin di�er hangi k��elere ba�land�klar�n� ve yol uzunluklar�n� biliyoruz.
	// Bunlar� kullanarak vertice[1]'den(labirentteki hedef kare) vertice[0]'a(labirentin ba�lang�� karesi)
	// en k�sa yolu bulmaya �al��aca��m. connection[0] y y�n�, connection[1] v y�n�, connection[2] x y�n�, connection[3] c y�n�.
	// ba�lamadan �nce yolu tuttu�umuz dizilerin t�m elemanlar�n� 51 yap�yoruz.
	// 51 o indexde k��e yok demek.
	// ve distance de�i�kenlerini ba�lang�� i�in ayarl�yoruz.
	for(i=0; i<50; i++){
		path[i] = 51;
		shortest_path[i] = 51;
	}
	distance = 0;
	shortest_distance = 145;
	path[0] = 1; // en k�sa yolun ba�lang�c� vertice dizisinin 1 indexli eleman�. Yani labirentin hedef noktas�.
	for(i=0; i<number_of_vertices; i++){
		vertice[i].checked = 0;
	}
	
	path_comparator(1, 1); // en k�sa yolu bulan fonksiyon
	
	// en k�sa yol uzunlu�unu ve yolu ekrana yaz�yoruz.
	printf("\n\n%d\n\n",shortest_distance);
	i=0;
	while(path[i] != 51){
		printf("%d\n",shortest_path[i]);
		i++;
	}
	//En k�sa yolu bulduk �imdi buna g�re kare bilgilerini g�ncellicez.
	//K��elerin en k�sa yola gitmeyen ge�itlerini kapat�caz.
	//B�ylece elimizde sadece tek bir yol kalm�� olacak ba�lang�� ve biti� aras�nda.
	//Robot da bu yoldan gidecek.
	//En k�sa yol hedef kareden ba�lay�p ba�lang�� karesine kadar s�r�yor.
	i=0;
	while(shortest_path[i] != 0){
		//connection[0] y y�n�, connection[1] v y�n�, connection[2] x y�n�, connection[3] c y�n�.
		
		if(vertice[shortest_path[i]].connection[0] != shortest_path[i+1] && vertice[shortest_path[i]].connection[0] != shortest_path[i-1]){
			square[vertice[shortest_path[i]].column][vertice[shortest_path[i]].line].y_direction = 0;
		}
		if(vertice[shortest_path[i]].connection[1] != shortest_path[i+1] && vertice[shortest_path[i]].connection[1] != shortest_path[i-1]){
			square[vertice[shortest_path[i]].column][vertice[shortest_path[i]].line].v_direction = 0;
		}
		if(vertice[shortest_path[i]].connection[2] != shortest_path[i+1] && vertice[shortest_path[i]].connection[2] != shortest_path[i-1]){
			square[vertice[shortest_path[i]].column][vertice[shortest_path[i]].line].x_direction = 0;
		}
		if(vertice[shortest_path[i]].connection[3] != shortest_path[i+1] && vertice[shortest_path[i]].connection[3] != shortest_path[i-1]){
			square[vertice[shortest_path[i]].column][vertice[shortest_path[i]].line].c_direction = 0;
		}
	i++;
	}
	//square dizisinde ge�itler �zerinde de�i�iklik yapt�k ve bir ge�it 2 farkl� de�i�kende tutuluyordu
	//ama biz bu ikisinden birini 0 yapt�k o y�zden daha �nce yapt���m�z gibi bu iki de�i�keni e�itlicez.
	for (i=0; i<=10; i++){
		for(a=0; a<=10; a++){ 
			if(square[i][a].x_direction == 0 || square[i][a+1].c_direction == 0){
				square[i][a+1].c_direction = 0;
				square[i][a].x_direction = 0;
			}
			if(square[i][a].v_direction == 0 || square[i+1][a].y_direction == 0){
				square[i+1][a].y_direction = 0;
				square[i][a].v_direction = 0;
			}
		}
	}
	hamle++;
	//sim�lasyonde g�rmek i�in yazd�r�yoruz
	write3();
}
int path_comparator(int v_index, int path_index){
	int a,i;
	for(a=0; a<=3; a++){ // o k��enin ge�itlerini kontrol ediyoruz ba�ka bir k��eye ba�lan�yor mu diye.
		if(vertice[v_index].path_distance[a] > 0){ // ba�lant� var
			if(vertice[vertice[v_index].connection[a]].checked == 1){
				// bu k��e daha �nce yola kat�ld�.
				// bir �ey yapma d�ng� devam etsin.
			}
			else if(vertice[v_index].connection[a] == 0){
				distance += vertice[v_index].path_distance[a];
				path[path_index] = vertice[v_index].connection[a];
				// labirentin ba�lang�� noktas�na geldik
				// yol olu�mu� oldu.
				if(distance < shortest_distance){
					shortest_distance = distance;
					for(i=0; i<50; i++){
						shortest_path[i] = path[i];	
					}
				}
				distance -= vertice[v_index].path_distance[a];
				vertice[v_index].checked = 0;
			}
			else{
				distance += vertice[v_index].path_distance[a];
				path[path_index] = vertice[v_index].connection[a];
				vertice[v_index].checked = 1;
				if(path_comparator(vertice[v_index].connection[a], path_index+1) == 1){
					distance -= vertice[v_index].path_distance[a];
					vertice[v_index].checked = 0;
				}
			}
		}
	}
	return 1;
}
int check_y(int column, int line, int vertice_index, int connection_index, int path_distance){
	int i;
	int connection_found = 0;
	if(square[column][line].y_direction == 1){
		// y y�n� a��k ise o kareye ge�iyoruz ve o karenin k��e olup olmad���n� kontrol ediyoruz.
		path_distance++;
		column -= 1;
		for(i=0; i<number_of_vertices; i++){
			if(column == vertice[i].column && line == vertice[i].line){
				// ba�ka bir k��eye ula�t�k.
				// k��emiz vertice dizisindeki i indexli k��e ile ba�lant�l�.
				vertice[vertice_index].connection[connection_index] = i;
				vertice[vertice_index].path_distance[connection_index] = path_distance;
				connection_found = 1;
				return 1;
			}
		}
		if(connection_found == 0){
			// yol a��k ama geldi�imiz kare k��e de�il ise ilerlemeye devam ediyoruz.
			if(check_y(column, line, vertice_index, connection_index, path_distance) == 0){
				// y kapal� ise x i kontrol ediyoruz
				if(check_x(column, line, vertice_index, connection_index, path_distance) == 0){
					// x kapal� ise c yi kontrol ediyoruz.
					if(check_c(column, line, vertice_index, connection_index, path_distance) == 1){ // c kapal� olmamal�.
						return 1;
					}
				}
				else
				return 1;
			}
			else
			return 1;
		}
	}
	else{
		// y y�n� kapal�.
		return 0;
	}
}

int check_v(int column, int line, int vertice_index, int connection_index, int path_distance){
	int i;
	int connection_found = 0;
	if(square[column][line].v_direction == 1){
		// v y�n� a��k ise o kareye ge�iyoruz ve o karenin k��e olup olmad���n� kontrol ediyoruz.
		path_distance++;
		column += 1;
		for(i=0; i<number_of_vertices; i++){
			if(column == vertice[i].column && line == vertice[i].line){
				// ba�ka bir k��eye ula�t�k.
				// k��emiz vertice dizisindeki i indexli k��e ile ba�lant�l�.
				vertice[vertice_index].connection[connection_index] = i;
				vertice[vertice_index].path_distance[connection_index] = path_distance;
				connection_found = 1;
				return 1;
			}
		}
		if(connection_found == 0){
			// yol a��k ama geldi�imiz kare k��e de�il ise ilerlemeye devam ediyoruz.
			if(check_v(column, line, vertice_index, connection_index, path_distance) == 0){
				// v kapal� ise x i kontrol ediyoruz
				if(check_x(column, line, vertice_index, connection_index, path_distance) == 0){
					// x kapal� ise c yi kontrol ediyoruz.
					if(check_c(column, line, vertice_index, connection_index, path_distance) == 1){ // c kapal� olmamal�.
						return 1;
					}
				}
				else
				return 1;
			}
			else
			return 1;
		}
	}
	else{
		// v y�n� kapal�.
		return 0;
	}
}

int check_x(int column, int line, int vertice_index, int connection_index, int path_distance){
	int i;
	int connection_found = 0;
	if(square[column][line].x_direction == 1){
		// x y�n� a��k ise o kareye ge�iyoruz ve o karenin k��e olup olmad���n� kontrol ediyoruz.
		path_distance++;
		line += 1;
		for(i=0; i<number_of_vertices; i++){
			if(column == vertice[i].column && line == vertice[i].line){
				// ba�ka bir k��eye ula�t�k.
				// k��emiz vertice dizisindeki i indexli k��e ile ba�lant�l�.
				vertice[vertice_index].connection[connection_index] = i;
				vertice[vertice_index].path_distance[connection_index] = path_distance;
				connection_found = 1;
				return 1;
			}
		}
		if(connection_found == 0){
			// yol a��k ama geldi�imiz kare k��e de�il ise ilerlemeye devam ediyoruz.
			if(check_x(column, line, vertice_index, connection_index, path_distance) == 0){
				// x kapal� ise y i kontrol ediyoruz
				if(check_y(column, line, vertice_index, connection_index, path_distance) == 0){
					// y kapal� ise v yi kontrol ediyoruz.
					if(check_v(column, line, vertice_index, connection_index, path_distance) == 1){ // v kapal� olmamal�.
						return 1;
					}
				}
				else
				return 1;
			}
			else
			return 1;
		}
	}
	else{
		// x y�n� kapal�.
		return 0;
	}
}
int check_c(int column, int line, int vertice_index, int connection_index, int path_distance){
	int i;
	int connection_found = 0;
	if(square[column][line].c_direction == 1){
		// c y�n� a��k ise o kareye ge�iyoruz ve o karenin k��e olup olmad���n� kontrol ediyoruz.
		path_distance++;
		line -= 1;
		for(i=0; i<number_of_vertices; i++){
			if(column == vertice[i].column && line == vertice[i].line){
				// ba�ka bir k��eye ula�t�k.
				// k��emiz vertice dizisindeki i indexli k��e ile ba�lant�l�.
				vertice[vertice_index].connection[connection_index] = i;
				vertice[vertice_index].path_distance[connection_index] = path_distance;
				connection_found = 1;
				return 1;
			}
		}
		if(connection_found == 0){
			// yol a��k ama geldi�imiz kare k��e de�il ise ilerlemeye devam ediyoruz.
			if(check_c(column, line, vertice_index, connection_index, path_distance) == 0){
				// c kapal� ise y i kontrol ediyoruz
				if(check_y(column, line, vertice_index, connection_index, path_distance) == 0){
					// y kapal� ise v yi kontrol ediyoruz.
					if(check_v(column, line, vertice_index, connection_index, path_distance) == 1){ // v kapal� olmamal�.
						return 1;
					}
				}
				else
				return 1;
			}
			else
			return 1;
		}
	}
	else{
		// c y�n� kapal�.
		return 0;
	}
}

void go_back_to_first_section(){
	while(1){
		descripe_robot();
		
		if(robot.column == 0 && robot.line == 0){ // e�er ba�lang�ca ula�t�ysak d�ng�y� bitiriyoruz.
			break;
		}
		
		choose_and_turn();
		hamle++;
		write2(); // G�ncel labirent bilgilerini dosyaya yaz�yoruz. 
		go_straight();
		write(); // Hamleleri ekrana ve dosyaya yaz�yoruz.
	}
	
}
void go_to_target_section(){
	while(1){
		descripe_robot();
		
		if(square[robot.column][robot.line].finish == 1){ // e�er ba�lang�ca ula�t�ysak d�ng�y� bitiriyoruz.
			break;
		}
		
		choose_and_turn();
		hamle++;
		write2(); // G�ncel labirent bilgilerini dosyaya yaz�yoruz. 
		go_straight();
		write(); // Hamleleri ekrana ve dosyaya yaz�yoruz.
	}
	
}
void write(){
	/*int delay;
	system("cls");
	int i,a;
	for(i=0; i<=24; i++){
		for(a=0; a<=24; a++){
			if(i == robot.column*2+1 && a == robot.line*2+1)
				printf("a");
			else
				printf("%c",maze[i][a]);
		}
		printf("\n");
	}
	for(delay=0; delay<=100000000; delay++){
	}*/
	
	printf("Hamle %d: ",hamle);
	printf("%d %d\n\n",robot.column,robot.line);
	
	
	//printf("Direction:    %c\n",robot.direction);
	//printf("On:           %d\n",robot.front);
	//printf("Sag:          %d\n",robot.right);
	//printf("Sol:          %d\n\n",robot.left);
	
	FILE *filep;
	filep = fopen("maze runner.txt","a");
	fprintf(filep,"%c",robot.direction);
	fprintf(filep,"  Hamle %d\n",hamle);
	fclose(filep);
	
	FILE *filep3;
	filep3 = fopen("Hamle Sayisi.txt","w");
	fprintf(filep3,"%d",hamle);
	fclose(filep3);
	
}
void write2(){
	// robot ��kmaza giden yollar� kapat�yor. Bunu sim�lasyonda da g�rmek i�in bunlar� yap�yoruz.
	if(square[robot.column][robot.line].y_direction == 0){
		maze2[robot.column*2][robot.line*2+1] = 'o';
	}
	if(square[robot.column][robot.line].v_direction == 0){
		maze2[robot.column*2+2][robot.line*2+1] = 'o';
	}
	if(square[robot.column][robot.line].x_direction == 0){
		maze2[robot.column*2+1][robot.line*2+2] = 'o';
	}
	if(square[robot.column][robot.line].c_direction == 0){
		maze2[robot.column*2+1][robot.line*2] = 'o';
	}
	// robot ziyaret edilen kareleri i�aretliyor. Bunu sim�lasyonda da g�rmek i�in bunlar� yap�yoruz.
	if(square[robot.column][robot.line].visit > 0){
		maze2[robot.column*2+1][robot.line*2+1] = '.'; // ziyaret edilen kareleri . olarak yaz�yoruz dosyaya
	}
	FILE *filep2;
	filep2 = fopen("maze.txt","a");
	int d,g;
	for(d=0; d<25; d++){
		for(g=0; g<25; g++){
			fprintf(filep2,"%c",maze2[d][g]);
		}
		fprintf(filep2,"\n");
	}
	fclose(filep2);
}
void write3(){
	int d,g;
	for(d=0; d<=11; d++){
		for(g=0; g<=11; g++){
			if(square[d][g].y_direction == 0){
				maze2[d*2][g*2+1] = 'o';
			}
			if(square[d][g].v_direction == 0){
				maze2[d*2+2][g*2+1] = 'o';
			}
			if(square[d][g].x_direction == 0){
				maze2[d*2+1][g*2+2] = 'o';
			}
			if(square[d][g].c_direction == 0){
				maze2[d*2+1][g*2] = 'o';
			}
		}
	}
	
	FILE *filep2;
	filep2 = fopen("maze.txt","a");
	for(d=0; d<25; d++){
		for(g=0; g<25; g++){
			fprintf(filep2,"%c",maze2[d][g]);
		}
		fprintf(filep2,"\n");
	}
	fclose(filep2);
	
	FILE *filep3;
	filep3 = fopen("Hamle Sayisi.txt","w");
	fprintf(filep3,"%d",hamle);
	fclose(filep3);
}
