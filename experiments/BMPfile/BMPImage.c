//Write on a "bmp.bmp" file and make a 256x256 image
// a checkerboard pattern black and white

//System call x86 open, write, close
long syscall(long number, ...);
#define SYS_close 3
#define SYS_open 2
#define SYS_write 1
#define O_WRONLY 1

/* Bitmap Header 14 bytes
Siganture -> 2 bytes
File Size -> 4 bytes
Reserved -> 4 bytes
Data Offset -> 4 bytes
*/

unsigned char bmp_header[14] = {
    0x42, 0x4D, //Siganture "BM"
    0x36, 0x00, 0x03, 0x00, //File size 196662 bytes total
    0x00, 0x00, //Reserved
    0x00, 0x00, //Reserved
    0x36, 0x00, 0x00, 0x00 //Data Offset 54

};

/* DIB Header (information) 40 bytes (commonly)
Header Size -> 4 bytes
Width -> 4 bytes 
Height -> 4 bytes
Planes -> 2 bytes
Bits per Pixel -> 2 bytes 
Compression -> 4 bytes
Image Size -> 4 bytes
Res & Colors -> 16 bytes
*/
unsigned char dib_header[40] = {
    0x28, 0x00, 0x00, 0x00, //Header Size 40 bytes
    0x00, 0x01, 0x00, 0x00, //Width 256 pixels
    0x00, 0x01, 0x00, 0x00, //Height 256 pixels
    0x01, 0x00, //Planes 
    0x18, 0x00, //Bits per pixel 24 bit color
    0x00, 0x00, 0x00, 0x00, //Compression
    0x00, 0x00, 0x03, 0x00, //Image Size
    0x13, 0x0B, 0x00, 0x00, //Resolution & Color
    0x13, 0x0B, 0x00, 0x00, //Resolution & Color
    0x00, 0x00, 0x00, 0x00, //Resolution & Color
    0x00, 0x00, 0x00, 0x00 //Resolution & Color
};

/* Pixel Array
Color order
Row Padding
Orientation
(R,G,B)
(0x00, 0x00, 0xFF) -> Red pixel
(0xFF, 0xFF, 0xFF)-> White pixel
(0x00, 0x00,) ->  padding
(0xFF, 0x00, 0x00,) -> Blue pixel
(0x00, 0xFF, 0x00,) -> Green Pixel
(0x00, 0x00,) ->  padding
*/
unsigned char pixel_array[256 * 256 * 3];

int main(){

    int row = 1;
    for (int i = 0; i < 256 * 256; i++) {
        if (i % 256 == 0){
            row++;
        }

        if (row % 2 == 0){
            if (i % 2 == 0){
                pixel_array[i * 3 + 0] = 0x00; // B
                pixel_array[i * 3 + 1] = 0x00; // G
                pixel_array[i * 3 + 2] = 0x00; // R
            }
            else {
                pixel_array[i * 3 + 0] = 0xFF; // B
                pixel_array[i * 3 + 1] = 0xFF; // G
                pixel_array[i * 3 + 2] = 0xFF; // R
            }
        }
        else{
            if (i % 2 == 0){
                pixel_array[i * 3 + 0] = 0xFF; // B
                pixel_array[i * 3 + 1] = 0xFF; // G
                pixel_array[i * 3 + 2] = 0xFF; // R
            }
            else {
                pixel_array[i * 3 + 0] = 0x00; // B
                pixel_array[i * 3 + 1] = 0x00; // G
                pixel_array[i * 3 + 2] = 0x00; // R
            }
        }
    }

    // "Abre" el file 
    int fd = syscall(SYS_open, "bmp.bmp", O_WRONLY);
    // Escribe en el file
    syscall(SYS_write, fd, bmp_header, 14);
    syscall(SYS_write, fd, dib_header, 40);
    syscall(SYS_write, fd, pixel_array, 256*256*3);
    //Cierra el file
    syscall(SYS_close, fd);

    return 0;

}










