template <typename TYPE, int SIZE> class doubleBuffer{
    public:
        volatile TYPE *readBuffer, *writeBuffer;
        void swap(){
            volatile TYPE *temp = readBuffer;
            readBuffer = writeBuffer;
            writeBuffer = temp;
        }
        void alloc(){
            readBuffer = (TYPE*)malloc(SIZE*sizeof(TYPE));
            writeBuffer = (TYPE*)malloc(SIZE*sizeof(TYPE));
        }
};

template <typename TYPE, int SIZE> class fftBuffer{
    public:
        void write(const TYPE *data, int w_size){
            int i_start = end_index;
            for(int j = 0, i = end_index; j < w_size; j++, i = (i+1)%SIZE) buffer[i] = data[j];
            end_index = (i_start+w_size)%SIZE;
        }
        void read(TYPE *data){
            int i_start = end_index-SIZE;
            if(i_start < 0) i_start += SIZE;
            for(int j = 0, i = i_start; j < SIZE; j++, i = (i+1)%SIZE) data[j] = buffer[i];
        }
        void alloc(){ buffer = (TYPE*)calloc(SIZE, sizeof(TYPE)); }
    private:
        TYPE *buffer;
        int end_index = 0;
};