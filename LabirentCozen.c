#include<stdio.h>
#include <stdbool.h>

/*
	Algoritma içerisinde bazý yorumlar çok anlaþýlýr olmayabilir.
	Örneðin ileride köþe diye bir terimden bahsedilmekte.
	Burada bahsedilen geometrik þekillerden bildiðimiz köþe ile ayný deðil.
	Buradaki köþenin tanýmý bir karenin 2'den fazla yol baðlantýsýna sahip olmasýdýr.
	Köþe denilmesinin nedeni algoritma içerisindeki en kýsa yolu hesaplama bölümü ile alakalý.
	Bu gibi anlamý biraz kapalý olan kýsýmlar hakkýnda açýklama dosyasý yazýlmasý planlanmakta.
*/

/* 12x12 olmak üzere 144 kareden oluþan bir labirent için yazýyorum kodu.
   Algoritma iþleyiþi:
   Robot her karede sað,sol ve öndeki sensörlerden gelen veri ile yol açýk mý yoksa duvar mý var bunu algýlar.
   Eðer sadece bir taraf açýksa o karede zaten tek yol vardýr ve ordan gitmek zorunludur. Eðer birden fazla taraf açýksa
   o kare için belli karar mekanizmalarý kullanmak gerekiyor.
   Karelerin bilgilerini tutacak deðiþkenleri içeren bir yapý tanýmlayýp o tipte 2 boyutlu bir dizi tanýmlanacak.
   Bu dizi kare bilgilerini bellekte tutmak ve gerekli iþlemleri yapmak için kullanýlacak.
   
   Labirentin x-y düzleminde olduðunu varsayýyorum ve robotun baþlangýç noktasýný (0,0) kabul ediyorum. Robotun
   baþlangýçta baktýðý yön, +y yönü kabul ediliyor. Bu yön ve konum bilgileri kullanýlarak robotu hareket ettirme
   ve konumunu belirleme iþlemleri yapýlacak.
*/

/*
 Bu maze yapýsý labirent haritasýný tutacak olan dizi için oluþturuldu. Burada bir nevi simülasyon programý yazýldýðý için
 labirent haritasýný programa elle girmek gerekiyor. Kodu robota uyarlarken bu yapý ve dizi kaldýrýlacak.
 Örneðin gerçekte ileri yol açýk mý bilgisi öndeki sensörden gelen veri ile anlaþýlacak. Burada ise tanýmlanacak olan maze
 dizisinin ilgili nesnesi kontrol edilerek anlaþýlacak. Yapý içinde 5 tane nesne olacak +y -y +x -x ve hedef. Bu nesneler ilgili karenin
 o yönlerinde duvar var mý yok mu bilgisini tutuyor olacak ve hedef o karenin ulaþýlmak istenen kare olup olmadýðý bilgisini tutacak.
 Açýkcasý labirent bilgilerini girmek uzun ve zahmetli bir iþ olduðu için program her baþladýðýnda kullanýcýnýn bunu yapmasý
 hoþ olmaz. O yüzden burada maze isimli bir labirent giricem. Labirent deðiþtirilmek istenirse kodun içinde burasý deðiþtirilmeli. 
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
// robot gezerken ziyaret ettiði kareleri iþaretleme,
// çýkmaza giden yollarý kapatma gibi iþlemler yapýyor.
// bunlarý python simülasyonunda görmek için bu ikinci maze 
// dizisini kullanýyoruz.
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


// Karelerin bilgilerini tutan yapý.
struct square_inf{
	int visit; // Kareye daha önce kaç kere gelindiðini tutan deðiþken.
	int y_direction; // Herhangi bir karenin 4 tarafýndaki geçit için açýk mý yoksa kapalý mý olduðunu tutan deðiþkenler.
	int v_direction; // 1 açýk, 0 kapalý, 2 görülmedi demek.
	int x_direction; // Labirenti 2 boyutlu x-y eksenine oturtuyoruz ve yönleri þu þekilde kodluyoruz.
	int c_direction; // +y yönü için y, -y için v, +x için x, -x için c kullanýyoruz.
	bool finish; // O karenin ulaþýlmak istenen kare olup olmadýðýný tutan deðiþken. 1 evet 0 hayýr.
	
	
};
struct square_inf square[12][12];

// Robot bilgilerini tutan yapý
struct robot_inf{
	// yön ve konum deðiþkenleri.
	// yön için +y = y, -y = v, +x = x, -x = c olarak kullanýlacak.
	// konum square dizisine göre bulunduðumuz karenin sütun ve satýr numaralarý.
	char direction;
	int column; // robotun labirentteki konumu. Sutun ve satýr olarak.
	int line;
	int front; // robotun ön sað ve solundaki geçitlerin açýk mý kapalý mý olduðunu tutan deðiþkenler
	int right;
	int left;
	int front_visit; // robotun ön sað ve solundaki karelerin ziyaret edilme sayýlarýný tutan deðiþkenler
	int right_visit;
	int left_visit;
};
struct robot_inf robot;

// En kýsa yolu bulmaya çalýþýrken yapacaðýmýz iþlemler için köþelerin bilgilerini tutacak olan yapý.
struct vertice_inf{
	int column;
	int line;
	int number_of_connections; // kaç farklý köþeye baðlanýyor.
	int connection[4]; // hangi köþelere baðlandýðýný tuttuðumuz deðiþkenler.
                       // Köþelerin isimleri = vertice dizisindeki indexleri.
                       // connection[0] y yönü, connection[1] v yönü, connection[2] x yönü, connection[3] c yönü.
	int path_distance[4]; // baðlantý yollarýnýn uzunluklarý. connection dizisi ile ayný indexli
	int checked; // en kýsa yolu hesaplarken o köþeye uðrayýp uðramadýðýmýzý kontrol etmek için bu deðiþkeni kontrol edicez. 
};
struct vertice_inf vertice[50]; // kaç tane köþe olacaðýný bilemiyorum. 50'den fazla olmaz diye düþünüyorum. 50 bile baya abartý.
int number_of_vertices; // köþe sayýsý
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
	// Python üzerinden simülasyon yapmak için verileri dosyalara yazýyoruz.
	// Sonra o verileri pythonda dosyadan okuyarak simülasyon yapýyoruz.
	// Kodu robota aktarýrken dosya iþlemleri silinecek.
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
	// Baþlangýç için bu yönü ve konumu kabul ediyoruz.
	// Robotun labirente nasýl koyulduðunun önemi yok. 4 köþeden biri olmasý yeterli.
	// Ancak eðer köþe deðilse kodda biraz oynama yapmak gerek çünkü C'de arraylere negatif
	// index verilmiyor.
	robot.direction = 'y';
	robot.column = 0;
	robot.line = 0;
	robot.front_visit = 0;
	robot.right_visit = 0;
	robot.left_visit = 0;
	
	// baþlangýçta tüm kareleri ziyaret edilmedi ve bitiþ deðil iþaretliyoruz.
	// Ve tüm karelerin 4 tarafýndaki geçitleri görülmedi olarak iþaretliyoruz. 
	int i,a; 
	for(i=0; i<=11; i++){
		for(a=0; a<=11; a++){
			square[i][a].visit=0;
			square[i][a].finish=0;
			square[i][a].y_direction = 2; // 1 açýk, 0 kapalý, 2 görülmedi demek.
			square[i][a].c_direction = 2;
			square[i][a].v_direction = 2;
			square[i][a].x_direction = 2;
		}
	}
	
	
	// Hedef kareyi bulana kadar dönecek olan döngü
	while(1){
		descripe_square();
		descripe_robot();
		
		if(square[robot.column][robot.line].finish == 1){ // eðer hedefe ulaþtýysak döngüyü bitiriyoruz.
			break;
		}
		
		choose_and_turn();
		hamle++;
		write2(); // Güncel labirent bilgilerini dosyaya yazýyoruz. 
		go_straight();
		write(); // Hamleleri ekrana ve dosyaya yazýyoruz.
	}
}

void descripe_square(){
	// burada normalde sensörlerden gelen verilere göre iþlem yapýcaz.
	// asýl konumun maze hali: square +1 +1
	int column_maze = robot.column*2+1;
	int line_maze = robot.line*2+1;
	if(square[robot.column][robot.line].visit == 0){
		// daha önce ziyaret edilmedi ise 1 kere ziyaret edildi olarak iþaretliyoruz 
		// ve karenin 4 tarafýndaki geçitleri kontrol ediyoruz açýk mý kapalý mý diye.
		square[robot.column][robot.line].visit += 1;
		if(maze[column_maze][line_maze] == 'x'){
			square[robot.column][robot.line].finish = 1; // eðer x ise orasý hedef karedir.
		}
		// baktýðý yöne göre, maze dizisindeki kontrol edeceði eleman deðiþiyor
		// o yüzden her yön için ayrý ayrý yazýyoruz.
		if(robot.direction == 'y'){
			// arka zaten açýktýr.
			square[robot.column][robot.line].v_direction = 1;
			//ön için: maze -1 0
			if(maze[column_maze-1][line_maze] == 'o'){
				square[robot.column][robot.line].y_direction = 0;
			}
			else if(maze[column_maze-1][line_maze] == ' ' || maze[column_maze-1][line_maze] == 'x'){
				square[robot.column][robot.line].y_direction = 1;
			}
			//sað için: maze 0 +1
			if(maze[column_maze][line_maze+1] == 'o'){
				square[robot.column][robot.line].x_direction = 0;
			}
			else if(maze[column_maze][line_maze+1] == ' ' || maze[column_maze][line_maze+1] == 'x'){
				square[robot.column][robot.line].x_direction = 1;
			}
			//sol için: maze 0 -1
			if(maze[column_maze][line_maze-1] == 'o'){
				square[robot.column][robot.line].c_direction = 0;
			}
			else if(maze[column_maze][line_maze-1] == ' ' || maze[column_maze][line_maze-1] == 'x'){
				square[robot.column][robot.line].c_direction = 1;
			}
		}
		else if(robot.direction == 'v'){
			// arka zaten açýktýr.
			square[robot.column][robot.line].y_direction = 1;
			//ön için: maze +1 0
			if(maze[column_maze+1][line_maze] == 'o'){
				square[robot.column][robot.line].v_direction = 0;
			}
			else if(maze[column_maze+1][line_maze] == ' ' || maze[column_maze+1][line_maze] == 'x'){
				square[robot.column][robot.line].v_direction = 1;
			}
			//sað için: maze 0 -1
			if(maze[column_maze][line_maze-1] == 'o'){
				square[robot.column][robot.line].c_direction = 0;
			}
			else if(maze[column_maze][line_maze-1] == ' ' || maze[column_maze][line_maze-1] == 'x'){
				square[robot.column][robot.line].c_direction = 1;
			}
			//sol için: maze 0 +1
			if(maze[column_maze][line_maze+1] == 'o'){
				square[robot.column][robot.line].x_direction = 0;
			}
			else if(maze[column_maze][line_maze+1] == ' ' || maze[column_maze][line_maze+1] == 'x'){
				square[robot.column][robot.line].x_direction = 1;
			}
		}
		else if(robot.direction == 'x'){
			// arkda zaten açýktýr.
			square[robot.column][robot.line].c_direction = 1;
			//ön için: maze 0 +1
			if(maze[column_maze][line_maze+1] == 'o'){
				square[robot.column][robot.line].x_direction = 0;
			}
			else if(maze[column_maze][line_maze+1] == ' ' || maze[column_maze][line_maze+1] == 'x'){
				square[robot.column][robot.line].x_direction = 1;
			}
			//sað için: maze +1 0
			if(maze[column_maze+1][line_maze] == 'o'){
				square[robot.column][robot.line].v_direction = 0;
			}
			else if(maze[column_maze+1][line_maze] == ' ' || maze[column_maze+1][line_maze] == 'x'){
				square[robot.column][robot.line].v_direction = 1;
			}
			//sol için: maze -1 0
			if(maze[column_maze-1][line_maze] == 'o'){
				square[robot.column][robot.line].y_direction = 0;
			}
			else if(maze[column_maze-1][line_maze] == ' ' || maze[column_maze-1][line_maze] == 'x'){
				square[robot.column][robot.line].y_direction = 1;
			}
		}
		else if(robot.direction == 'c'){
			// arka zaten açýktýr.
			square[robot.column][robot.line].x_direction = 1;
			//ön için: maze 0 -1
			if(maze[column_maze][line_maze-1] == 'o'){
				square[robot.column][robot.line].c_direction = 0;
			}
			else if(maze[column_maze][line_maze-1] == ' ' || maze[column_maze][line_maze-1] == 'x'){
				square[robot.column][robot.line].c_direction = 1;
			}
			//sað için: maze -1 0
			if(maze[column_maze-1][line_maze] == 'o'){
				square[robot.column][robot.line].y_direction = 0;
			}
			else if(maze[column_maze-1][line_maze] == ' ' || maze[column_maze-1][line_maze] == 'x'){
				square[robot.column][robot.line].y_direction = 1;
			}
			//sol için: maze +1 0
			if(maze[column_maze+1][line_maze] == 'o'){
				square[robot.column][robot.line].v_direction = 0;
			}
			else if(maze[column_maze+1][line_maze] == ' ' || maze[column_maze+1][line_maze] == 'x'){
				square[robot.column][robot.line].v_direction = 1;
			}
		}
	}
	else{
		// Eðer daha önce ziyaret edildi ise tekrar 4 tarafý kontrol etmeye gerek yok.
		// Visit deðiþkenini 1 arttýrýyoruz.
		// Çýkmaz yoldan geliyor olma ihtimalimiz var. O yüzden bir önceki karenin çýkmaz olup olmadýðýný
		// kontrol edicez. Eðer çýkmazsa yani 3 tarafý kapalý ise geldiðimiz geçiti de kapalý iþaretlicez.
		// Böylece robot oraya tekrar gelirse orayý kapalý görecek ve girmicek. Çýkmazdan dönerken
		// bu iþlemi yapmaya devam ettiðimiz için çýkmaza giden tüm yollar kapalý iþaretlenmiþ olacak.
		square[robot.column][robot.line].visit += 1;
		if(robot.direction == 'y'){
			 // bir önceki karenin çýkmaz olup olmadýðýný kontrol ediyoruz.
			if(square[robot.column+1][robot.line].v_direction == 0 && square[robot.column+1][robot.line].x_direction == 0 && square[robot.column+1][robot.line].c_direction == 0){
				// eðer öyleyse ve o kare baþlangýç karesi deðilse geldiðimiz geçiti kapalý olarak iþaretliyoruz.
				if(robot.column+1 != 0 || robot.line != 0){
					square[robot.column][robot.line].v_direction = 0;
				}
			}
		}
		else if(robot.direction == 'v'){
			 // bir önceki karenin çýkmaz olup olmadýðýný kontrol ediyoruz.
			if(square[robot.column-1][robot.line].y_direction == 0 && square[robot.column-1][robot.line].x_direction == 0 && square[robot.column-1][robot.line].c_direction == 0){
				// eðer öyleyse ve o kare baþlangýç karesi deðilse geldiðimiz geçiti kapalý olarak iþaretliyoruz.
				if(robot.column-1 != 0 || robot.line != 0){
					square[robot.column][robot.line].y_direction = 0;
				}
			}
		}
		else if(robot.direction == 'x'){
			 // bir önceki karenin çýkmaz olup olmadýðýný kontrol ediyoruz.
			if(square[robot.column][robot.line-1].c_direction == 0 && square[robot.column][robot.line-1].y_direction == 0 && square[robot.column][robot.line-1].v_direction == 0){
				// eðer öyleyse ve o kare baþlangýç karesi deðilse geldiðimiz geçiti kapalý olarak iþaretliyoruz.
				if(robot.column != 0 || robot.line-1 != 0){
					square[robot.column][robot.line].c_direction = 0;
				}
			}
		}
		else if(robot.direction == 'c'){
			 // bir önceki karenin çýkmaz olup olmadýðýný kontrol ediyoruz.
			if(square[robot.column][robot.line+1].x_direction == 0 && square[robot.column][robot.line+1].y_direction == 0 && square[robot.column][robot.line+1].v_direction == 0){
				// eðer öyleyse ve o kare baþlangýç karesi deðilse geldiðimiz geçiti kapalý olarak iþaretliyoruz.
				if(robot.column != 0 || robot.line+1 != 0){
					square[robot.column][robot.line].x_direction = 0;
				}
			}
		}
	}
}
void descripe_robot(){
	// Kare bilgilerini robotun o anki yönüne göre robot deðiþkenlerine aktarýyoruz.
	// Böylece karar verme fonksiyonumuzda robotun yönünü düþünmemize gerek kalmýyor.
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
	// geçitlerin açýklýk durumuna göre hangi yöne gideceðimizi seçeceðiz.
	// Tek geçit açýksa ordan gideceðiz. Birden fazla ise öncelik sýrasýna göre seçeceðiz.
	if(robot.front == 0 && robot.right == 0 && robot.left == 0){
		turn_around();
	}
	else if(robot.front == 0 && robot.right == 0){
		//tek dönüþ var
		turn_left();
	}
	else if(robot.front == 0 && robot.left == 0){
		//tek dönüþ var
		turn_right();
	}
	else if(robot.right == 0 && robot.left == 0){
		//sadece ön açýk o yüzden dönmüyoruz
	}
	// Bundan sonraki ihtimallerde iki tane açýk geçit var.
	// Bu iki karenin ziyaret edilme sayýsýný kontrol etmeliyiz.
	// Ziyaret edilme sayýsý daha az olan kareye gideceðiz.
	else if(robot.front == 0){
		// sað ve sol açýk.
		if(robot.right_visit <= robot.left_visit){
			turn_right();
		}
		else{
			turn_left();
		}
	}
	else if(robot.right == 0){
		//ön ve sol açýk
		if(robot.front_visit <= robot.left_visit){
			//dönmüyoruz
		}
		else{
			turn_left();
		}
	}
	else if(robot.left == 0){
		//ön ve sað açýk
		if(robot.front_visit <= robot.right_visit){
			//dönmüyoruz
		}
		else{
			turn_right();
		}
	}
	else{
		//tüm yönler açýk
		if(robot.front_visit <= robot.right_visit && robot.front_visit <= robot.left_visit){
			// dönmüyoruz
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
	// Bu fonksiyon robot hedef kareyi bulduktan sonra çalýþacak.
	// Baþlangýç ile bitiþ arasýndaki en kýsa yolu bulucaz.
	// Bunu yaparken ziyaret edilmiþ olan karelere göre iþlem yapacaðýz.
	// Ziyaret edilmeyen karelere giden geçitleri kapalý kabul edeceðiz.
	// En kýsa yol ziyaret edilmeyen karelerden geçiyor da olabilir ancak
	// biz bunu bu aþamada bilemiyoruz. Ýlerleyen zamanlarda bunun için de bir
	// çözüm üretmeliyiz. Þimdilik bu kodu tamamlayýp elimizde çalýþan bir kod olmasý
	// üzerine yoðunlaþýyorum.
	
	// Ziyaret edilmeyen karelere giden geçitleri kapalý olarak iþaretliyoruz.
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
	//simülasyonde görmek için yazdýrýyoruz
	write3();
	
	// Program bir geçiti iki farklý deðiþkende tutuyor. Örneðin 0 0 karesinin x yönündeki geçiti
	// square[0][0].x_direction deðiþkeni ile square[0][1].c_direction deðiþkenleri tutuyor. Çýkmaz
	// yola giden geçitleri kapalý kabul ederken herhangi bir geçit için bu iki deðiþkenden bir tanesini
	// kapalý olarak kabul ettik. Aynýsýný ziyaret edilmeyen karelere giden geçitleri kapalý olarak
	// iþaretlerken de yaptýk. Þimdi burda her geçit için o iki deðiþkeni birbirine eþitleyeceðiz.
	// Sadece biri kapalý ise diðerini de kapalý yapýcaz. Ýkisi de kapalý veya açýksa deðiþiklik olmicak.	
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
	//simülasyonde görmek için yazdýrýyoruz
	write3();
	
	// Dijkstra en kýsa yol algoritmasýný yapmaya çalýþýyorum. Baþlangýçdan hedefe ulaþana kadar yaptýðým
	// haritalandýrma sonucunda 3 veya 4 tarafý açýk kalan kareler Köþe(Vertices) olacak. 
	// Baþlangýç ve hedef karelerinin  kaç tarafý açýk olduðunun önemi yok onlar da köþe olacak.
	// 2 tarafý açýk olan kareler köþeleri baðlayan yollardýr. Ýki köþe arasýndaki karelerin sayýsý yol uzunluðudur.
	// Labirentteki hedef karesini baþlangýç köþesi, baþlangýç karesini de bitiþ köþesi kabul ederek djikstra algoritmasýný
	// uygulamaya çalýþacaðým.
	
	// baþlangýç karesi ilk köþemiz.
	vertice[0].column = 0;
	vertice[0].line = 0;
	int wall[4]; // vertice[0]'ýn baðlantý sayýsý kontrol edilmeye baþlanýyor
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
	
	// Hedef kare ikinci köþemiz. hedef kareyi buluyoruz
	for(i=0; i<=11; i++){
		for(a=0; a<=11; a++){
			if(square[i][a].finish == 1){
				vertice[1].column = i;
				vertice[1].line = a;
			}
		}
	} // vertice [1]'in baðlantý sayýsýný kontrol etmeye baþlýyoruz
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
	
	number_of_vertices = 2; // þu ana kadar iki tane köþe tanýmladýk.
	
	// diðer köþeleri tanýmlýyoruz. Tüm kareleri tarýyorum bunlardan 3 veya 4 tarafý açýk olan kare köþedir.
	for(i=0; i<=11; i++){
		for(a=0; a<=11; a++){
			if((i == 0 && a == 0) || square[i][a].finish == 1){
				// bunlarý zaten tanýmladýk o yüzden bunlar için iþlem yapmýyoruz.
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
					// 3 geçit açýk yani bu kare köþe.
					number_of_vertices++;
					vertice[number_of_vertices-1].column = i;
					vertice[number_of_vertices-1].line = a;
					vertice[number_of_vertices-1].number_of_connections = 3;
				}
				else if(a2 == 4){
					// 4 geçit açýk yani bu kare köþe.
					number_of_vertices++;
					vertice[number_of_vertices-1].column = i;
					vertice[number_of_vertices-1].line = a;
					vertice[number_of_vertices-1].number_of_connections = 4;
				}
				a2 = 0; // kontrol sayacýmýzý sýfýrlýyoruz.
			}
		}
	}
	
	// köþeleri ekrana yazýyoruz
	
	printf("\n Kose sayisi: %d\n",number_of_vertices);
	for(i=0; i<number_of_vertices; i++){
		printf("Kose %d: %d %d. Baglanti sayisi: %d\n",i,vertice[i].column,vertice[i].line,vertice[i].number_of_connections);
	}
	
	// baðlantýlarý ve yol uzunluklarýný tanýmlýyoruz.
	// burada path_distance dizisinin bazý elemanlarý 0 kalacak.
	// oralardan baðlantý olmadýðý anlamýna geliyor.
	// connection dizisinin de baðlantý olmayan elemanlarý ve vertice[0]'a baðlanan
	// elemanlarý 0 olacak. o yüzden connection[x] sýfýrsa onun vertice[0]' baðlý olup olmadýðýný
	// anlamak için path_distance[x]'i de kontrol etmeliyiz. Eðer path_distance[x] sýfýr deðilse o baðlantý vardýr.
	for(i=0; i<number_of_vertices; i++){
		for(a=0; a<4; a++){
			vertice[i].path_distance[a] = 0; // baþlamadan önce tüm yol uzunluklarýný ve baðlantýlarý sýfýr yapýyoruz.
			vertice[i].connection[a] = 0;
		}
	}
	
	for(i=0; i<number_of_vertices; i++){
		//bu 4 fonksiyon o köþenin o yönündeki geçitten hangi köþeye baðlandýðýný
		//ve bu baðlantýnýn yol uzunluðunun ne kadar olduðunu buluyor.
		//her yön için farklý bir fonksiyon. 
		check_y(vertice[i].column, vertice[i].line, i, 0, 0); 
		check_v(vertice[i].column, vertice[i].line, i, 1, 0);
		check_x(vertice[i].column, vertice[i].line, i, 2, 0);
		check_c(vertice[i].column, vertice[i].line, i, 3, 0);
	}
	// baðlantýlarý ve yol uzunluklarýný ekrana yazýyoruz.
	
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
	
	// Artýk tüm köþeleri, köþelerin diðer hangi köþelere baðlandýklarýný ve yol uzunluklarýný biliyoruz.
	// Bunlarý kullanarak vertice[1]'den(labirentteki hedef kare) vertice[0]'a(labirentin baþlangýç karesi)
	// en kýsa yolu bulmaya çalýþacaðým. connection[0] y yönü, connection[1] v yönü, connection[2] x yönü, connection[3] c yönü.
	// baþlamadan önce yolu tuttuðumuz dizilerin tüm elemanlarýný 51 yapýyoruz.
	// 51 o indexde köþe yok demek.
	// ve distance deðiþkenlerini baþlangýç için ayarlýyoruz.
	for(i=0; i<50; i++){
		path[i] = 51;
		shortest_path[i] = 51;
	}
	distance = 0;
	shortest_distance = 145;
	path[0] = 1; // en kýsa yolun baþlangýcý vertice dizisinin 1 indexli elemaný. Yani labirentin hedef noktasý.
	for(i=0; i<number_of_vertices; i++){
		vertice[i].checked = 0;
	}
	
	path_comparator(1, 1); // en kýsa yolu bulan fonksiyon
	
	// en kýsa yol uzunluðunu ve yolu ekrana yazýyoruz.
	printf("\n\n%d\n\n",shortest_distance);
	i=0;
	while(path[i] != 51){
		printf("%d\n",shortest_path[i]);
		i++;
	}
	//En kýsa yolu bulduk þimdi buna göre kare bilgilerini güncellicez.
	//Köþelerin en kýsa yola gitmeyen geçitlerini kapatýcaz.
	//Böylece elimizde sadece tek bir yol kalmýþ olacak baþlangýç ve bitiþ arasýnda.
	//Robot da bu yoldan gidecek.
	//En kýsa yol hedef kareden baþlayýp baþlangýç karesine kadar sürüyor.
	i=0;
	while(shortest_path[i] != 0){
		//connection[0] y yönü, connection[1] v yönü, connection[2] x yönü, connection[3] c yönü.
		
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
	//square dizisinde geçitler üzerinde deðiþiklik yaptýk ve bir geçit 2 farklý deðiþkende tutuluyordu
	//ama biz bu ikisinden birini 0 yaptýk o yüzden daha önce yaptýðýmýz gibi bu iki deðiþkeni eþitlicez.
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
	//simülasyonde görmek için yazdýrýyoruz
	write3();
}
int path_comparator(int v_index, int path_index){
	int a,i;
	for(a=0; a<=3; a++){ // o köþenin geçitlerini kontrol ediyoruz baþka bir köþeye baðlanýyor mu diye.
		if(vertice[v_index].path_distance[a] > 0){ // baðlantý var
			if(vertice[vertice[v_index].connection[a]].checked == 1){
				// bu köþe daha önce yola katýldý.
				// bir þey yapma döngü devam etsin.
			}
			else if(vertice[v_index].connection[a] == 0){
				distance += vertice[v_index].path_distance[a];
				path[path_index] = vertice[v_index].connection[a];
				// labirentin baþlangýç noktasýna geldik
				// yol oluþmuþ oldu.
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
		// y yönü açýk ise o kareye geçiyoruz ve o karenin köþe olup olmadýðýný kontrol ediyoruz.
		path_distance++;
		column -= 1;
		for(i=0; i<number_of_vertices; i++){
			if(column == vertice[i].column && line == vertice[i].line){
				// baþka bir köþeye ulaþtýk.
				// köþemiz vertice dizisindeki i indexli köþe ile baðlantýlý.
				vertice[vertice_index].connection[connection_index] = i;
				vertice[vertice_index].path_distance[connection_index] = path_distance;
				connection_found = 1;
				return 1;
			}
		}
		if(connection_found == 0){
			// yol açýk ama geldiðimiz kare köþe deðil ise ilerlemeye devam ediyoruz.
			if(check_y(column, line, vertice_index, connection_index, path_distance) == 0){
				// y kapalý ise x i kontrol ediyoruz
				if(check_x(column, line, vertice_index, connection_index, path_distance) == 0){
					// x kapalý ise c yi kontrol ediyoruz.
					if(check_c(column, line, vertice_index, connection_index, path_distance) == 1){ // c kapalý olmamalý.
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
		// y yönü kapalý.
		return 0;
	}
}

int check_v(int column, int line, int vertice_index, int connection_index, int path_distance){
	int i;
	int connection_found = 0;
	if(square[column][line].v_direction == 1){
		// v yönü açýk ise o kareye geçiyoruz ve o karenin köþe olup olmadýðýný kontrol ediyoruz.
		path_distance++;
		column += 1;
		for(i=0; i<number_of_vertices; i++){
			if(column == vertice[i].column && line == vertice[i].line){
				// baþka bir köþeye ulaþtýk.
				// köþemiz vertice dizisindeki i indexli köþe ile baðlantýlý.
				vertice[vertice_index].connection[connection_index] = i;
				vertice[vertice_index].path_distance[connection_index] = path_distance;
				connection_found = 1;
				return 1;
			}
		}
		if(connection_found == 0){
			// yol açýk ama geldiðimiz kare köþe deðil ise ilerlemeye devam ediyoruz.
			if(check_v(column, line, vertice_index, connection_index, path_distance) == 0){
				// v kapalý ise x i kontrol ediyoruz
				if(check_x(column, line, vertice_index, connection_index, path_distance) == 0){
					// x kapalý ise c yi kontrol ediyoruz.
					if(check_c(column, line, vertice_index, connection_index, path_distance) == 1){ // c kapalý olmamalý.
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
		// v yönü kapalý.
		return 0;
	}
}

int check_x(int column, int line, int vertice_index, int connection_index, int path_distance){
	int i;
	int connection_found = 0;
	if(square[column][line].x_direction == 1){
		// x yönü açýk ise o kareye geçiyoruz ve o karenin köþe olup olmadýðýný kontrol ediyoruz.
		path_distance++;
		line += 1;
		for(i=0; i<number_of_vertices; i++){
			if(column == vertice[i].column && line == vertice[i].line){
				// baþka bir köþeye ulaþtýk.
				// köþemiz vertice dizisindeki i indexli köþe ile baðlantýlý.
				vertice[vertice_index].connection[connection_index] = i;
				vertice[vertice_index].path_distance[connection_index] = path_distance;
				connection_found = 1;
				return 1;
			}
		}
		if(connection_found == 0){
			// yol açýk ama geldiðimiz kare köþe deðil ise ilerlemeye devam ediyoruz.
			if(check_x(column, line, vertice_index, connection_index, path_distance) == 0){
				// x kapalý ise y i kontrol ediyoruz
				if(check_y(column, line, vertice_index, connection_index, path_distance) == 0){
					// y kapalý ise v yi kontrol ediyoruz.
					if(check_v(column, line, vertice_index, connection_index, path_distance) == 1){ // v kapalý olmamalý.
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
		// x yönü kapalý.
		return 0;
	}
}
int check_c(int column, int line, int vertice_index, int connection_index, int path_distance){
	int i;
	int connection_found = 0;
	if(square[column][line].c_direction == 1){
		// c yönü açýk ise o kareye geçiyoruz ve o karenin köþe olup olmadýðýný kontrol ediyoruz.
		path_distance++;
		line -= 1;
		for(i=0; i<number_of_vertices; i++){
			if(column == vertice[i].column && line == vertice[i].line){
				// baþka bir köþeye ulaþtýk.
				// köþemiz vertice dizisindeki i indexli köþe ile baðlantýlý.
				vertice[vertice_index].connection[connection_index] = i;
				vertice[vertice_index].path_distance[connection_index] = path_distance;
				connection_found = 1;
				return 1;
			}
		}
		if(connection_found == 0){
			// yol açýk ama geldiðimiz kare köþe deðil ise ilerlemeye devam ediyoruz.
			if(check_c(column, line, vertice_index, connection_index, path_distance) == 0){
				// c kapalý ise y i kontrol ediyoruz
				if(check_y(column, line, vertice_index, connection_index, path_distance) == 0){
					// y kapalý ise v yi kontrol ediyoruz.
					if(check_v(column, line, vertice_index, connection_index, path_distance) == 1){ // v kapalý olmamalý.
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
		// c yönü kapalý.
		return 0;
	}
}

void go_back_to_first_section(){
	while(1){
		descripe_robot();
		
		if(robot.column == 0 && robot.line == 0){ // eðer baþlangýca ulaþtýysak döngüyü bitiriyoruz.
			break;
		}
		
		choose_and_turn();
		hamle++;
		write2(); // Güncel labirent bilgilerini dosyaya yazýyoruz. 
		go_straight();
		write(); // Hamleleri ekrana ve dosyaya yazýyoruz.
	}
	
}
void go_to_target_section(){
	while(1){
		descripe_robot();
		
		if(square[robot.column][robot.line].finish == 1){ // eðer baþlangýca ulaþtýysak döngüyü bitiriyoruz.
			break;
		}
		
		choose_and_turn();
		hamle++;
		write2(); // Güncel labirent bilgilerini dosyaya yazýyoruz. 
		go_straight();
		write(); // Hamleleri ekrana ve dosyaya yazýyoruz.
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
	// robot çýkmaza giden yollarý kapatýyor. Bunu simülasyonda da görmek için bunlarý yapýyoruz.
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
	// robot ziyaret edilen kareleri iþaretliyor. Bunu simülasyonda da görmek için bunlarý yapýyoruz.
	if(square[robot.column][robot.line].visit > 0){
		maze2[robot.column*2+1][robot.line*2+1] = '.'; // ziyaret edilen kareleri . olarak yazýyoruz dosyaya
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
