#include<stdio.h>
#include <stdbool.h>

/*
	Algoritma içerisinde bazı yorumlar çok anlaşılır olmayabilir.
	Örneğin ileride köşe diye bir terimden bahsedilmekte.
	Burada bahsedilen geometrik şekillerden bildiğimiz köşe ile aynı değil.
	Buradaki köşenin tanımı bir karenin 2'den fazla yol bağlantısına sahip olmasıdır.
	Köşe denilmesinin nedeni algoritma içerisindeki en kısa yolu hesaplama bölümü ile alakalı.
	Bu gibi anlamı biraz kapalı olan kısımlar hakkında açıklama dosyası yazılması planlanmakta.
*/

/* 12x12 olmak üzere 144 kareden oluşan bir labirent için yazıyorum kodu.
   Algoritma işleyişi:
   Robot her karede sağ,sol ve öndeki sensörlerden gelen veri ile yol açık mı yoksa duvar mı var bunu algılar.
   Eğer sadece bir taraf açıksa o karede zaten tek yol vardır ve ordan gitmek zorunludur. Eğer birden fazla taraf açıksa
   o kare için belli karar mekanizmaları kullanmak gerekiyor.
   Karelerin bilgilerini tutacak değişkenleri içeren bir yapı tanımlayıp o tipte 2 boyutlu bir dizi tanımlanacak.
   Bu dizi kare bilgilerini bellekte tutmak ve gerekli işlemleri yapmak için kullanılacak.
   
   Labirentin x-y düzleminde olduğunu varsayıyorum ve robotun başlangıç noktasını (0,0) kabul ediyorum. Robotun
   başlangıçta baktığı yön, +y yönü kabul ediliyor. Bu yön ve konum bilgileri kullanılarak robotu hareket ettirme
   ve konumunu belirleme işlemleri yapılacak.
*/

/*
 Bu maze yapısı labirent haritasını tutacak olan dizi için oluşturuldu. Burada bir nevi simülasyon programı yazıldığı için
 labirent haritasını programa elle girmek gerekiyor. Kodu robota uyarlarken bu yapı ve dizi kaldırılacak.
 Örneğin gerçekte ileri yol açık mı bilgisi öndeki sensörden gelen veri ile anlaşılacak. Burada ise tanımlanacak olan maze
 dizisinin ilgili nesnesi kontrol edilerek anlaşılacak. Yapı içinde 5 tane nesne olacak +y -y +x -x ve hedef. Bu nesneler ilgili karenin
 o yönlerinde duvar var mı yok mu bilgisini tutuyor olacak ve hedef o karenin ulaşılmak istenen kare olup olmadığı bilgisini tutacak.
 Açıkcası labirent bilgilerini girmek uzun ve zahmetli bir iş olduğu için program her başladığında kullanıcının bunu yapması
 hoş olmaz. O yüzden burada maze isimli bir labirent giricem. Labirent değiştirilmek istenirse kodun içinde burası değiştirilmeli. 
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
// robot gezerken ziyaret ettiği kareleri işaretleme,
// çıkmaza giden yolları kapatma gibi işlemler yapıyor.
// bunları python simülasyonunda görmek için bu ikinci maze 
// dizisini kullanıyoruz.
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


// Karelerin bilgilerini tutan yapı.
struct square_inf{
	int visit; // Kareye daha önce kaç kere gelindiğini tutan değişken.
	int y_direction; // Herhangi bir karenin 4 tarafındaki geçit için açık mı yoksa kapalı mı olduğunu tutan değişkenler.
	int v_direction; // 1 açık, 0 kapalı, 2 görülmedi demek.
	int x_direction; // Labirenti 2 boyutlu x-y eksenine oturtuyoruz ve yönleri şu şekilde kodluyoruz.
	int c_direction; // +y yönü için y, -y için v, +x için x, -x için c kullanıyoruz.
	bool finish; // O karenin ulaşılmak istenen kare olup olmadığını tutan değişken. 1 evet 0 hayır.
	
	
};
struct square_inf square[12][12];

// Robot bilgilerini tutan yapı
struct robot_inf{
	// yön ve konum değişkenleri.
	// yön için +y = y, -y = v, +x = x, -x = c olarak kullanılacak.
	// konum square dizisine göre bulunduğumuz karenin sütun ve satır numaraları.
	char direction;
	int column; // robotun labirentteki konumu. Sutun ve satır olarak.
	int line;
	int front; // robotun ön sağ ve solundaki geçitlerin açık mı kapalı mı olduğunu tutan değişkenler
	int right;
	int left;
	int front_visit; // robotun ön sağ ve solundaki karelerin ziyaret edilme sayılarını tutan değişkenler
	int right_visit;
	int left_visit;
};
struct robot_inf robot;

// En kısa yolu bulmaya çalışırken yapacağımız işlemler için köşelerin bilgilerini tutacak olan yapı.
struct vertice_inf{
	int column;
	int line;
	int number_of_connections; // kaç farklı köşeye bağlanıyor.
	int connection[4]; // hangi köşelere bağlandığını tuttuğumuz değişkenler.
                       // Köşelerin isimleri = vertice dizisindeki indexleri.
                       // connection[0] y yönü, connection[1] v yönü, connection[2] x yönü, connection[3] c yönü.
	int path_distance[4]; // bağlantı yollarının uzunlukları. connection dizisi ile aynı indexli
	int checked; // en kısa yolu hesaplarken o köşeye uğrayıp uğramadığımızı kontrol etmek için bu değişkeni kontrol edicez. 
};
struct vertice_inf vertice[50]; // kaç tane köşe olacağını bilemiyorum. 50'den fazla olmaz diye düşünüyorum. 50 bile baya abartı.
int number_of_vertices; // köşe sayısı
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
	// Python üzerinden simülasyon yapmak için verileri dosyalara yazıyoruz.
	// Sonra o verileri pythonda dosyadan okuyarak simülasyon yapıyoruz.
	// Kodu robota aktarırken dosya işlemleri silinecek.
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
	// Başlangıç için bu yönü ve konumu kabul ediyoruz.
	// Robotun labirente nasıl koyulduğunun önemi yok. 4 köşeden biri olması yeterli.
	// Ancak eğer köşe değilse kodda biraz oynama yapmak gerek çünkü C'de arraylere negatif
	// index verilmiyor.
	robot.direction = 'y';
	robot.column = 0;
	robot.line = 0;
	robot.front_visit = 0;
	robot.right_visit = 0;
	robot.left_visit = 0;
	
	// başlangıçta tüm kareleri ziyaret edilmedi ve bitiş değil işaretliyoruz.
	// Ve tüm karelerin 4 tarafındaki geçitleri görülmedi olarak işaretliyoruz. 
	int i,a; 
	for(i=0; i<=11; i++){
		for(a=0; a<=11; a++){
			square[i][a].visit=0;
			square[i][a].finish=0;
			square[i][a].y_direction = 2; // 1 açık, 0 kapalı, 2 görülmedi demek.
			square[i][a].c_direction = 2;
			square[i][a].v_direction = 2;
			square[i][a].x_direction = 2;
		}
	}
	
	
	// Hedef kareyi bulana kadar dönecek olan döngü
	while(1){
		descripe_square();
		descripe_robot();
		
		if(square[robot.column][robot.line].finish == 1){ // eğer hedefe ulaştıysak döngüyü bitiriyoruz.
			break;
		}
		
		choose_and_turn();
		hamle++;
		write2(); // Güncel labirent bilgilerini dosyaya yazıyoruz. 
		go_straight();
		write(); // Hamleleri ekrana ve dosyaya yazıyoruz.
	}
}

void descripe_square(){
	// burada normalde sensörlerden gelen verilere göre işlem yapıcaz.
	// asıl konumun maze hali: square +1 +1
	int column_maze = robot.column*2+1;
	int line_maze = robot.line*2+1;
	if(square[robot.column][robot.line].visit == 0){
		// daha önce ziyaret edilmedi ise 1 kere ziyaret edildi olarak işaretliyoruz 
		// ve karenin 4 tarafındaki geçitleri kontrol ediyoruz açık mı kapalı mı diye.
		square[robot.column][robot.line].visit += 1;
		if(maze[column_maze][line_maze] == 'x'){
			square[robot.column][robot.line].finish = 1; // eğer x ise orası hedef karedir.
		}
		// baktığı yöne göre, maze dizisindeki kontrol edeceği eleman değişiyor
		// o yüzden her yön için ayrı ayrı yazıyoruz.
		if(robot.direction == 'y'){
			// arka zaten açıktır.
			square[robot.column][robot.line].v_direction = 1;
			//ön için: maze -1 0
			if(maze[column_maze-1][line_maze] == 'o'){
				square[robot.column][robot.line].y_direction = 0;
			}
			else if(maze[column_maze-1][line_maze] == ' ' || maze[column_maze-1][line_maze] == 'x'){
				square[robot.column][robot.line].y_direction = 1;
			}
			//sağ için: maze 0 +1
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
			// arka zaten açıktır.
			square[robot.column][robot.line].y_direction = 1;
			//ön için: maze +1 0
			if(maze[column_maze+1][line_maze] == 'o'){
				square[robot.column][robot.line].v_direction = 0;
			}
			else if(maze[column_maze+1][line_maze] == ' ' || maze[column_maze+1][line_maze] == 'x'){
				square[robot.column][robot.line].v_direction = 1;
			}
			//sağ için: maze 0 -1
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
			// arkda zaten açıktır.
			square[robot.column][robot.line].c_direction = 1;
			//ön için: maze 0 +1
			if(maze[column_maze][line_maze+1] == 'o'){
				square[robot.column][robot.line].x_direction = 0;
			}
			else if(maze[column_maze][line_maze+1] == ' ' || maze[column_maze][line_maze+1] == 'x'){
				square[robot.column][robot.line].x_direction = 1;
			}
			//sağ için: maze +1 0
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
			// arka zaten açıktır.
			square[robot.column][robot.line].x_direction = 1;
			//ön için: maze 0 -1
			if(maze[column_maze][line_maze-1] == 'o'){
				square[robot.column][robot.line].c_direction = 0;
			}
			else if(maze[column_maze][line_maze-1] == ' ' || maze[column_maze][line_maze-1] == 'x'){
				square[robot.column][robot.line].c_direction = 1;
			}
			//sağ için: maze -1 0
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
		// Eğer daha önce ziyaret edildi ise tekrar 4 tarafı kontrol etmeye gerek yok.
		// Visit değişkenini 1 arttırıyoruz.
		// Çıkmaz yoldan geliyor olma ihtimalimiz var. O yüzden bir önceki karenin çıkmaz olup olmadığını
		// kontrol edicez. Eğer çıkmazsa yani 3 tarafı kapalı ise geldiğimiz geçiti de kapalı işaretlicez.
		// Böylece robot oraya tekrar gelirse orayı kapalı görecek ve girmicek. Çıkmazdan dönerken
		// bu işlemi yapmaya devam ettiğimiz için çıkmaza giden tüm yollar kapalı işaretlenmiş olacak.
		square[robot.column][robot.line].visit += 1;
		if(robot.direction == 'y'){
			 // bir önceki karenin çıkmaz olup olmadığını kontrol ediyoruz.
			if(square[robot.column+1][robot.line].v_direction == 0 && square[robot.column+1][robot.line].x_direction == 0 && square[robot.column+1][robot.line].c_direction == 0){
				// eğer öyleyse ve o kare başlangıç karesi değilse geldiğimiz geçiti kapalı olarak işaretliyoruz.
				if(robot.column+1 != 0 || robot.line != 0){
					square[robot.column][robot.line].v_direction = 0;
				}
			}
		}
		else if(robot.direction == 'v'){
			 // bir önceki karenin çıkmaz olup olmadığını kontrol ediyoruz.
			if(square[robot.column-1][robot.line].y_direction == 0 && square[robot.column-1][robot.line].x_direction == 0 && square[robot.column-1][robot.line].c_direction == 0){
				// eğer öyleyse ve o kare başlangıç karesi değilse geldiğimiz geçiti kapalı olarak işaretliyoruz.
				if(robot.column-1 != 0 || robot.line != 0){
					square[robot.column][robot.line].y_direction = 0;
				}
			}
		}
		else if(robot.direction == 'x'){
			 // bir önceki karenin çıkmaz olup olmadığını kontrol ediyoruz.
			if(square[robot.column][robot.line-1].c_direction == 0 && square[robot.column][robot.line-1].y_direction == 0 && square[robot.column][robot.line-1].v_direction == 0){
				// eğer öyleyse ve o kare başlangıç karesi değilse geldiğimiz geçiti kapalı olarak işaretliyoruz.
				if(robot.column != 0 || robot.line-1 != 0){
					square[robot.column][robot.line].c_direction = 0;
				}
			}
		}
		else if(robot.direction == 'c'){
			 // bir önceki karenin çıkmaz olup olmadığını kontrol ediyoruz.
			if(square[robot.column][robot.line+1].x_direction == 0 && square[robot.column][robot.line+1].y_direction == 0 && square[robot.column][robot.line+1].v_direction == 0){
				// eğer öyleyse ve o kare başlangıç karesi değilse geldiğimiz geçiti kapalı olarak işaretliyoruz.
				if(robot.column != 0 || robot.line+1 != 0){
					square[robot.column][robot.line].x_direction = 0;
				}
			}
		}
	}
}
void descripe_robot(){
	// Kare bilgilerini robotun o anki yönüne göre robot değişkenlerine aktarıyoruz.
	// Böylece karar verme fonksiyonumuzda robotun yönünü düşünmemize gerek kalmıyor.
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
	// geçitlerin açıklık durumuna göre hangi yöne gideceğimizi seçeceğiz.
	// Tek geçit açıksa ordan gideceğiz. Birden fazla ise öncelik sırasına göre seçeceğiz.
	if(robot.front == 0 && robot.right == 0 && robot.left == 0){
		turn_around();
	}
	else if(robot.front == 0 && robot.right == 0){
		//tek dönüş var
		turn_left();
	}
	else if(robot.front == 0 && robot.left == 0){
		//tek dönüş var
		turn_right();
	}
	else if(robot.right == 0 && robot.left == 0){
		//sadece ön açık o yüzden dönmüyoruz
	}
	// Bundan sonraki ihtimallerde iki tane açık geçit var.
	// Bu iki karenin ziyaret edilme sayısını kontrol etmeliyiz.
	// Ziyaret edilme sayısı daha az olan kareye gideceğiz.
	else if(robot.front == 0){
		// sağ ve sol açık.
		if(robot.right_visit <= robot.left_visit){
			turn_right();
		}
		else{
			turn_left();
		}
	}
	else if(robot.right == 0){
		//ön ve sol açık
		if(robot.front_visit <= robot.left_visit){
			//dönmüyoruz
		}
		else{
			turn_left();
		}
	}
	else if(robot.left == 0){
		//ön ve sağ açık
		if(robot.front_visit <= robot.right_visit){
			//dönmüyoruz
		}
		else{
			turn_right();
		}
	}
	else{
		//tüm yönler açık
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
	// Bu fonksiyon robot hedef kareyi bulduktan sonra çalışacak.
	// Başlangıç ile bitiş arasındaki en kısa yolu bulucaz.
	// Bunu yaparken ziyaret edilmiş olan karelere göre işlem yapacağız.
	// Ziyaret edilmeyen karelere giden geçitleri kapalı kabul edeceğiz.
	// En kısa yol ziyaret edilmeyen karelerden geçiyor da olabilir ancak
	// biz bunu bu aşamada bilemiyoruz. İlerleyen zamanlarda bunun için de bir
	// çözüm üretmeliyiz. Şimdilik bu kodu tamamlayıp elimizde çalışan bir kod olması
	// üzerine yoğunlaşıyorum.
	
	// Ziyaret edilmeyen karelere giden geçitleri kapalı olarak işaretliyoruz.
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
	//simülasyonde görmek için yazdırıyoruz
	write3();
	
	// Program bir geçiti iki farklı değişkende tutuyor. Örneğin 0 0 karesinin x yönündeki geçiti
	// square[0][0].x_direction değişkeni ile square[0][1].c_direction değişkenleri tutuyor. Çıkmaz
	// yola giden geçitleri kapalı kabul ederken herhangi bir geçit için bu iki değişkenden bir tanesini
	// kapalı olarak kabul ettik. Aynısını ziyaret edilmeyen karelere giden geçitleri kapalı olarak
	// işaretlerken de yaptık. Şimdi burda her geçit için o iki değişkeni birbirine eşitleyeceğiz.
	// Sadece biri kapalı ise diğerini de kapalı yapıcaz. İkisi de kapalı veya açıksa değişiklik olmicak.	
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
	//simülasyonde görmek için yazdırıyoruz
	write3();
	
	// Dijkstra en kısa yol algoritmasını yapmaya çalışıyorum. Başlangıçdan hedefe ulaşana kadar yaptığım
	// haritalandırma sonucunda 3 veya 4 tarafı açık kalan kareler Köşe(Vertices) olacak. 
	// Başlangıç ve hedef karelerinin  kaç tarafı açık olduğunun önemi yok onlar da köşe olacak.
	// 2 tarafı açık olan kareler köşeleri bağlayan yollardır. İki köşe arasındaki karelerin sayısı yol uzunluğudur.
	// Labirentteki hedef karesini başlangıç köşesi, başlangıç karesini de bitiş köşesi kabul ederek djikstra algoritmasını
	// uygulamaya çalışacağım.
	
	// başlangıç karesi ilk köşemiz.
	vertice[0].column = 0;
	vertice[0].line = 0;
	int wall[4]; // vertice[0]'ın bağlantı sayısı kontrol edilmeye başlanıyor
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
	
	// Hedef kare ikinci köşemiz. hedef kareyi buluyoruz
	for(i=0; i<=11; i++){
		for(a=0; a<=11; a++){
			if(square[i][a].finish == 1){
				vertice[1].column = i;
				vertice[1].line = a;
			}
		}
	} // vertice [1]'in bağlantı sayısını kontrol etmeye başlıyoruz
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
	
	number_of_vertices = 2; // şu ana kadar iki tane köşe tanımladık.
	
	// diğer köşeleri tanımlıyoruz. Tüm kareleri tarıyorum bunlardan 3 veya 4 tarafı açık olan kare köşedir.
	for(i=0; i<=11; i++){
		for(a=0; a<=11; a++){
			if((i == 0 && a == 0) || square[i][a].finish == 1){
				// bunları zaten tanımladık o yüzden bunlar için işlem yapmıyoruz.
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
					// 3 geçit açık yani bu kare köşe.
					number_of_vertices++;
					vertice[number_of_vertices-1].column = i;
					vertice[number_of_vertices-1].line = a;
					vertice[number_of_vertices-1].number_of_connections = 3;
				}
				else if(a2 == 4){
					// 4 geçit açık yani bu kare köşe.
					number_of_vertices++;
					vertice[number_of_vertices-1].column = i;
					vertice[number_of_vertices-1].line = a;
					vertice[number_of_vertices-1].number_of_connections = 4;
				}
				a2 = 0; // kontrol sayacımızı sıfırlıyoruz.
			}
		}
	}
	
	// köşeleri ekrana yazıyoruz
	
	printf("\n Kose sayisi: %d\n",number_of_vertices);
	for(i=0; i<number_of_vertices; i++){
		printf("Kose %d: %d %d. Baglanti sayisi: %d\n",i,vertice[i].column,vertice[i].line,vertice[i].number_of_connections);
	}
	
	// bağlantıları ve yol uzunluklarını tanımlıyoruz.
	// burada path_distance dizisinin bazı elemanları 0 kalacak.
	// oralardan bağlantı olmadığı anlamına geliyor.
	// connection dizisinin de bağlantı olmayan elemanları ve vertice[0]'a bağlanan
	// elemanları 0 olacak. o yüzden connection[x] sıfırsa onun vertice[0]' bağlı olup olmadığını
	// anlamak için path_distance[x]'i de kontrol etmeliyiz. Eğer path_distance[x] sıfır değilse o bağlantı vardır.
	for(i=0; i<number_of_vertices; i++){
		for(a=0; a<4; a++){
			vertice[i].path_distance[a] = 0; // başlamadan önce tüm yol uzunluklarını ve bağlantıları sıfır yapıyoruz.
			vertice[i].connection[a] = 0;
		}
	}
	
	for(i=0; i<number_of_vertices; i++){
		//bu 4 fonksiyon o köşenin o yönündeki geçitten hangi köşeye bağlandığını
		//ve bu bağlantının yol uzunluğunun ne kadar olduğunu buluyor.
		//her yön için farklı bir fonksiyon. 
		check_y(vertice[i].column, vertice[i].line, i, 0, 0); 
		check_v(vertice[i].column, vertice[i].line, i, 1, 0);
		check_x(vertice[i].column, vertice[i].line, i, 2, 0);
		check_c(vertice[i].column, vertice[i].line, i, 3, 0);
	}
	// bağlantıları ve yol uzunluklarını ekrana yazıyoruz.
	
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
	
	// Artık tüm köşeleri, köşelerin diğer hangi köşelere bağlandıklarını ve yol uzunluklarını biliyoruz.
	// Bunları kullanarak vertice[1]'den(labirentteki hedef kare) vertice[0]'a(labirentin başlangıç karesi)
	// en kısa yolu bulmaya çalışacağım. connection[0] y yönü, connection[1] v yönü, connection[2] x yönü, connection[3] c yönü.
	// başlamadan önce yolu tuttuğumuz dizilerin tüm elemanlarını 51 yapıyoruz.
	// 51 o indexde köşe yok demek.
	// ve distance değişkenlerini başlangıç için ayarlıyoruz.
	for(i=0; i<50; i++){
		path[i] = 51;
		shortest_path[i] = 51;
	}
	distance = 0;
	shortest_distance = 145;
	path[0] = 1; // en kısa yolun başlangıcı vertice dizisinin 1 indexli elemanı. Yani labirentin hedef noktası.
	for(i=0; i<number_of_vertices; i++){
		vertice[i].checked = 0;
	}
	
	path_comparator(1, 1); // en kısa yolu bulan fonksiyon
	
	// en kısa yol uzunluğunu ve yolu ekrana yazıyoruz.
	printf("\n\n%d\n\n",shortest_distance);
	i=0;
	while(path[i] != 51){
		printf("%d\n",shortest_path[i]);
		i++;
	}
	//En kısa yolu bulduk şimdi buna göre kare bilgilerini güncellicez.
	//Köşelerin en kısa yola gitmeyen geçitlerini kapatıcaz.
	//Böylece elimizde sadece tek bir yol kalmış olacak başlangıç ve bitiş arasında.
	//Robot da bu yoldan gidecek.
	//En kısa yol hedef kareden başlayıp başlangıç karesine kadar sürüyor.
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
	//square dizisinde geçitler üzerinde değişiklik yaptık ve bir geçit 2 farklı değişkende tutuluyordu
	//ama biz bu ikisinden birini 0 yaptık o yüzden daha önce yaptığımız gibi bu iki değişkeni eşitlicez.
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
	//simülasyonde görmek için yazdırıyoruz
	write3();
}
int path_comparator(int v_index, int path_index){
	int a,i;
	for(a=0; a<=3; a++){ // o köşenin geçitlerini kontrol ediyoruz başka bir köşeye bağlanıyor mu diye.
		if(vertice[v_index].path_distance[a] > 0){ // bağlantı var
			if(vertice[vertice[v_index].connection[a]].checked == 1){
				// bu köşe daha önce yola katıldı.
				// bir şey yapma döngü devam etsin.
			}
			else if(vertice[v_index].connection[a] == 0){
				distance += vertice[v_index].path_distance[a];
				path[path_index] = vertice[v_index].connection[a];
				// labirentin başlangıç noktasına geldik
				// yol oluşmuş oldu.
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
		// y yönü açık ise o kareye geçiyoruz ve o karenin köşe olup olmadığını kontrol ediyoruz.
		path_distance++;
		column -= 1;
		for(i=0; i<number_of_vertices; i++){
			if(column == vertice[i].column && line == vertice[i].line){
				// başka bir köşeye ulaştık.
				// köşemiz vertice dizisindeki i indexli köşe ile bağlantılı.
				vertice[vertice_index].connection[connection_index] = i;
				vertice[vertice_index].path_distance[connection_index] = path_distance;
				connection_found = 1;
				return 1;
			}
		}
		if(connection_found == 0){
			// yol açık ama geldiğimiz kare köşe değil ise ilerlemeye devam ediyoruz.
			if(check_y(column, line, vertice_index, connection_index, path_distance) == 0){
				// y kapalı ise x i kontrol ediyoruz
				if(check_x(column, line, vertice_index, connection_index, path_distance) == 0){
					// x kapalı ise c yi kontrol ediyoruz.
					if(check_c(column, line, vertice_index, connection_index, path_distance) == 1){ // c kapalı olmamalı.
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
		// y yönü kapalı.
		return 0;
	}
}

int check_v(int column, int line, int vertice_index, int connection_index, int path_distance){
	int i;
	int connection_found = 0;
	if(square[column][line].v_direction == 1){
		// v yönü açık ise o kareye geçiyoruz ve o karenin köşe olup olmadığını kontrol ediyoruz.
		path_distance++;
		column += 1;
		for(i=0; i<number_of_vertices; i++){
			if(column == vertice[i].column && line == vertice[i].line){
				// başka bir köşeye ulaştık.
				// köşemiz vertice dizisindeki i indexli köşe ile bağlantılı.
				vertice[vertice_index].connection[connection_index] = i;
				vertice[vertice_index].path_distance[connection_index] = path_distance;
				connection_found = 1;
				return 1;
			}
		}
		if(connection_found == 0){
			// yol açık ama geldiğimiz kare köşe değil ise ilerlemeye devam ediyoruz.
			if(check_v(column, line, vertice_index, connection_index, path_distance) == 0){
				// v kapalı ise x i kontrol ediyoruz
				if(check_x(column, line, vertice_index, connection_index, path_distance) == 0){
					// x kapalı ise c yi kontrol ediyoruz.
					if(check_c(column, line, vertice_index, connection_index, path_distance) == 1){ // c kapalı olmamalı.
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
		// v yönü kapalı.
		return 0;
	}
}

int check_x(int column, int line, int vertice_index, int connection_index, int path_distance){
	int i;
	int connection_found = 0;
	if(square[column][line].x_direction == 1){
		// x yönü açık ise o kareye geçiyoruz ve o karenin köşe olup olmadığını kontrol ediyoruz.
		path_distance++;
		line += 1;
		for(i=0; i<number_of_vertices; i++){
			if(column == vertice[i].column && line == vertice[i].line){
				// başka bir köşeye ulaştık.
				// köşemiz vertice dizisindeki i indexli köşe ile bağlantılı.
				vertice[vertice_index].connection[connection_index] = i;
				vertice[vertice_index].path_distance[connection_index] = path_distance;
				connection_found = 1;
				return 1;
			}
		}
		if(connection_found == 0){
			// yol açık ama geldiğimiz kare köşe değil ise ilerlemeye devam ediyoruz.
			if(check_x(column, line, vertice_index, connection_index, path_distance) == 0){
				// x kapalı ise y i kontrol ediyoruz
				if(check_y(column, line, vertice_index, connection_index, path_distance) == 0){
					// y kapalı ise v yi kontrol ediyoruz.
					if(check_v(column, line, vertice_index, connection_index, path_distance) == 1){ // v kapalı olmamalı.
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
		// x yönü kapalı.
		return 0;
	}
}
int check_c(int column, int line, int vertice_index, int connection_index, int path_distance){
	int i;
	int connection_found = 0;
	if(square[column][line].c_direction == 1){
		// c yönü açık ise o kareye geçiyoruz ve o karenin köşe olup olmadığını kontrol ediyoruz.
		path_distance++;
		line -= 1;
		for(i=0; i<number_of_vertices; i++){
			if(column == vertice[i].column && line == vertice[i].line){
				// başka bir köşeye ulaştık.
				// köşemiz vertice dizisindeki i indexli köşe ile bağlantılı.
				vertice[vertice_index].connection[connection_index] = i;
				vertice[vertice_index].path_distance[connection_index] = path_distance;
				connection_found = 1;
				return 1;
			}
		}
		if(connection_found == 0){
			// yol açık ama geldiğimiz kare köşe değil ise ilerlemeye devam ediyoruz.
			if(check_c(column, line, vertice_index, connection_index, path_distance) == 0){
				// c kapalı ise y i kontrol ediyoruz
				if(check_y(column, line, vertice_index, connection_index, path_distance) == 0){
					// y kapalı ise v yi kontrol ediyoruz.
					if(check_v(column, line, vertice_index, connection_index, path_distance) == 1){ // v kapalı olmamalı.
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
		// c yönü kapalı.
		return 0;
	}
}

void go_back_to_first_section(){
	while(1){
		descripe_robot();
		
		if(robot.column == 0 && robot.line == 0){ // eğer başlangıca ulaştıysak döngüyü bitiriyoruz.
			break;
		}
		
		choose_and_turn();
		hamle++;
		write2(); // Güncel labirent bilgilerini dosyaya yazıyoruz. 
		go_straight();
		write(); // Hamleleri ekrana ve dosyaya yazıyoruz.
	}
	
}
void go_to_target_section(){
	while(1){
		descripe_robot();
		
		if(square[robot.column][robot.line].finish == 1){ // eğer başlangıca ulaştıysak döngüyü bitiriyoruz.
			break;
		}
		
		choose_and_turn();
		hamle++;
		write2(); // Güncel labirent bilgilerini dosyaya yazıyoruz. 
		go_straight();
		write(); // Hamleleri ekrana ve dosyaya yazıyoruz.
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
	// robot çıkmaza giden yolları kapatıyor. Bunu simülasyonda da görmek için bunları yapıyoruz.
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
	// robot ziyaret edilen kareleri işaretliyor. Bunu simülasyonda da görmek için bunları yapıyoruz.
	if(square[robot.column][robot.line].visit > 0){
		maze2[robot.column*2+1][robot.line*2+1] = '.'; // ziyaret edilen kareleri . olarak yazıyoruz dosyaya
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
