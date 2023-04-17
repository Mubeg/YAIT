typedef unsigned int data_t;
typedef unsigned long long int pos_t;
typedef unsigned long long int my_time_t; // time in couting delay units


const char settings_filename[] = "settings.txt";

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

namespace nikitadanilov
{

    void mysleep(my_time_t ms)
    {
        for(int i = 0; i < ms; i++){
            for(int j = 0; j < 100; j++){}
        }
    }
    struct Times
    {
        my_time_t write_delay = 100;
        my_time_t read_delay = 100;
        my_time_t next_delay = 100;
        my_time_t setpos_delay = 100;
        Times()
        {
            ifstream file(settings_filename);
            if(!file){
                cout << "Невозможно открыть файл " << settings_filename << endl;
            }
            else
            {
                file >> write_delay >> read_delay >> next_delay >> setpos_delay;
            }
        }
    } times {};

    

    class Tape final
    {
        private:
            string name;
            pos_t maxsize;
            pos_t pos;
            vector<data_t> data;
        public:
            Tape(pos_t size, char * filename) : name{filename}, maxsize{size}
            {
                ifstream file(name);
                data.reserve(maxsize);
                if(!file){
                    cout << "Невозможно открыть файл " << name << endl;
                }
                else
                {
                    for(pos_t i = 0; i < maxsize; i++){
                        data_t temp;
                        file >> temp;
                        if(file.eof())
                        {
                            break;
                        }
                        data[i] = temp;
                    }
                }
            }
            ~Tape()
            {
                ofstream file(name);
                if(!file){
                    cout << "Невозможно открыть файл " << name << endl;
                }
                else
                {
                    for(pos_t i = 0; i < maxsize; i++){
                        file << data[i] << "\n";
                    }
                }
            }
            void write(data_t elem)
            {
                #ifndef NO_SLEEP
                usleep(times.write_delay);
                #endif
                #ifdef FAST_SLEEP
                mysleep(times.write_delay);
                #endif
                data[pos] = elem;
            }
            data_t read(void)
            {
                #ifndef NO_SLEEP
                usleep(times.read_delay);
                #endif
                #ifdef FAST_SLEEP
                mysleep(times.read_delay);
                #endif
                return data[pos];
            }
            void change_pos(pos_t position, bool forward_direction=true)
            {
                if(position == 1)
                {
                    next_pos();
                }
                else
                {   
                    #ifndef NO_SLEEP
                    usleep(times.setpos_delay);
                    #endif
                    #ifdef FAST_SLEEP
                    mysleep(times.setpos_delay);
                    #endif
                    pos += forward_direction ? position : -position;
                    pos %= maxsize;
                }
            }
            void next_pos(void)
            {
                #ifndef NO_SLEEP
                usleep(times.next_delay);
                #endif
                #ifdef FAST_SLEEP
                mysleep(times.next_delay);
                #endif
                pos++;
                pos %= maxsize;
            }
            void set_pos(pos_t position)
            {
                change_pos(position - pos);
            }
    };
    
    
    class TapeSort
    {
        private:
        pos_t n;
        pos_t m; //Not used yet
        public:
        void sort(Tape& in, Tape& out)
        {
            data_t min{0}, temp{0}, max{0};
            pos_t min_pos{0}, prev_min_pos{0};

            max = in.read();
            for(pos_t j = 0; j < n; j++, in.next_pos())
            {
                temp = in.read();
                if(max < temp){max = temp;}
            }

            for(pos_t i = 0; i < n; i++, out.next_pos())
            {
                min = max;
                min_pos = 0;
                for(pos_t j = 0; j < n; j++, in.next_pos())
                {
                    temp = in.read();
                    if(j == prev_min_pos && i != 0)
                    {
                        in.write(max);
                    }
                    else if(min > temp)
                    {
                        min = temp;
                        min_pos = j;
                    }
                }
                #ifdef LONG_WAIT
                if(i % 100 == 0){
                    cout << "Sorting " << i << " out of " << n << endl;
                }
                #endif
                prev_min_pos = min_pos;
                out.write(min);
            }
        }
        TapeSort(pos_t n, pos_t m): n{n}, m{m}{}
    };

};

using namespace nikitadanilov;


int main(int argc, char* argv[])
{
    if(argc != 5){
        cout << "Неверное использование программы\nПример правильного использования:\nTapeSort n m input.file output.file\n"; 
        return 0;
    }

    pos_t n = atoll(argv[1]), m = atoll(argv[2]);

    Tape in(n, argv[3]);
    Tape out(n, argv[4]);

    TapeSort(n, m).sort(in, out);

    return 0;
}