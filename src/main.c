#include "../libs/syscalls.h"
#include "../libs/buffer.h"

void alphabet(){
    Buffer *buff = buff_init(0);
    char x = 97;
    while(1){
        if(x >= 123){
            x = 97;
        }
        buff_insert(buff, &x, sizeof(char), 0);
        bin_dump(buff_body(buff), buff_size(buff), LITTLE_ENDIAN);
        buff_clear(buff);
        x++;
    }
}

void hills(){
    byte ones = 0xff;
    Buffer *buff = buff_init(sizeof(ones));
    int height = 10;
    for(int i = 0; i < height; i ++){
        buff_append_byte(buff, ones);
    }
    
    size_t i = 1;
    int k = 0;
    int bluntness = 6;
    int direction = 1;

    while(1){
        k ++;
        buff_dump(buff, i, LITTLE_ENDIAN);
        if(direction){
            if(k % bluntness == 0){
                i ++;
            }
            if(i == buff_capacity(buff)){
                direction = !direction;
            }
        }else{
            if(k % bluntness == 0){
                i --;
            }
            if(i == 0){
                direction = !direction;
            }
        }
    }

        
    
}

int main(){
    print(STDOUT_FILENO, "Hello World!%d\n", 25);
    println(STDERR_FILENO, "This is my message!");

    hills();
    // alphabet();

    return 0;
}