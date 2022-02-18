# LabirentCozen

## Proje Açıklaması
Bu projede C dilinde labirent çözen bir algoritma yazılması amaçlandı.
Algoritma fiziksel bir robot üzerinde uygulanmak üzere tasarlandı.
Algoritmayı test etmek amacıyla C kodunun başında 2 boyutlu maze isimli bir dizi tanımlandı.
Bu dizi çözülmesi gereken labirenti temsil etmektedir.
Programda sanal bir robot bu dizi üzerinde gezerek labirenti tanımakta ve çözmeye çalışmaktadır.
Labirent çözüldükten sonra elde edilen bilgiler "Maze runner", "maze" ve "Hamle sayısı" isimli metin dosyalarına yazılır, sanal robot başlangıç ile bitiş arasında birkaç kez tur atar ve program sona erer.
Ayrıca C dili ile yazılan algoritmanın testlerine görsellik kazandırmak için python ile bir simülatör yazıldı. Bu simülatör metin dosyalarından verileri okuyarak bunları renkli bir arayüz üzerinde kullanıcıya sunar.

Fiziksel robotun önünde, sağında ve solunda olmak üzere 3 adet mesafe sensörüne sahip olacağı ve labirent içerisinde bulunduğu konumun çevresindeki duvarları bu sensörler yardımıyla algılayacağı düşünüldü. Algoritma fiziksel robota uyarlanacağı zaman dosya işlemleri ile maze isimli dizinin kaldırılması ve robotun bulundu karenin çevresindeki duvarları maze dizisinden değil mesafe sensörlerinden okuması için birkaç fonksiyonda değişiklik yapılması gerekmektedir.

## Proje Videosu
Simülatöre ait bir video kaydının linki: https://youtu.be/FJNTcwh57Jg

## Projeyi Çalıştırabilmeniz için:
Dosyaları indirdikten sonra önce LabirentCozen.c dosyasını derleyip çalıştırmalısınız. Dosya içerinde en başta tanımlanan labirenti algoritma çözecek ve çıktıları metin dosyalarına yazacak. Ardından simulasyon.py dosyasını çalıştırarak algoritmanın çıktılarını grafiksel ortamda görebilirsiniz. Python dosyasının çalışabilmesi için pygame kütüphanesinin yüklü olması gerekmektedir.
