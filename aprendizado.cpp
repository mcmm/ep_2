
#include <cekeikon.h>
#include <cstdlib>
#include <iostream>
#include <fstream>


//In other words, the first byte is the label of the first image, which is a number in the range 0-9. 
//The next 3072 bytes are the values of the pixels of the image. The first 1024 bytes are the red channel values, 
//the next 1024 the green, and the final 1024 the blue. The values are stored in row-major order, 
//so the first 32 bytes are the red channel values of the first row of the image. 

void ler_bin(const string& filename, vector<int>& labels, vector<Mat>& images){
	bool finish = false;

		//Referências
		  //http://www.cplusplus.com/doc/tutorial/files/
		streampos label_size;
		streampos image_size;
		streampos size;

	  	char * label; //represents the address of an array 
	  					//of bytes where the read data elements are stored or from where the data elements to be written are taken
	  	char * image;

	  	ifstream file (filename, ios::in|ios::binary|ios::ate); //the file is open with the ios::ate flag, 
	  															//which means that the get pointer will be positioned 
	  																//at the end of the file. 
	  	if (file.is_open()) {
	  		size = file.tellg(); //when we call to member tellg(), we will directly obtain the size of the file.
	    	label_size = 1;
	    	image_size = 3072;
	    	int posicao = 0;

	    	int file_size = (int)size;
	    	printf("total %d\n",file_size);
	    	while(!finish){
	    		printf("%d", posicao);
	    		if(posicao==file_size) {
	    			finish = true;
	    			printf("terminei\n");
	    		}
	    		label = new char [label_size]; //we request the allocation of a memory block large enough to hold the entire file
	    		image = new char [image_size];

	    	
	    		file.seekg (posicao, ios::beg); // we proceed to set the get position at the beginning of the file 
	    			    							//(remember that we opened the file with this pointer at the end)
	    		file.read (label, label_size); //then we read the file
	    		
	    		labels.push_back(atoi(label));
	    			    	
	    		posicao+=sizeof(label);
	    		file.seekg (posicao, ios::beg);
	    		file.read (image, image_size);
	    	
	    		Mat_<COR> imagem(32,32);
	    					
	    		int aux =0;
	    		for (int l = 0; l < imagem.rows; ++l) {
	    			for (int c = 0; c < imagem.cols; ++c) {
	    				imagem(l, c)[0] = (int)image[aux+2047];
	    				imagem(l, c)[1] = (int)image[aux+1024];
	    				imagem(l, c)[2] = (int)image[aux];
	    					
	    				aux++; 
	    			}
	    		}
	    		images.push_back(imagem);
	    		delete[] label;
	    		delete[] image;
	    	}

		    file.close(); //and finally close it:
		  	}

		  	else cout << "Unable to open file";

	}

int main(int argc, char const *argv[])
{
	std::vector<int> labels; //vetor que salva as classificações das imagens
	std::vector<Mat> images; //vetor que salva as imagens

	ler_bin(argv[1], labels, images);

	
}