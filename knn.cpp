
#include <cekeikon.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
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
	    	
	    	int file_size = (int)size;
	    	int total_images = file_size/(label_size+image_size);
	    	int imagem_atual =0;
	    	
	    	while(!finish){
	    		if(imagem_atual==total_images) {
	    			finish = true;
	    		}

	    		int posicao = 3073*imagem_atual;
	    		
	    		label = new char [label_size]; //we request the allocation of a memory block large enough to hold the entire file
	    		image = new char [image_size];

	    	
	    		file.seekg (posicao, ios::beg); // we proceed to set the get position at the beginning of the file 
	    			    							//(remember that we opened the file with this pointer at the end)
	    		file.read (label, label_size); //then we read the file
	    		
				int label_int = static_cast<int>( *label);
	    		labels.push_back(label_int);
	    			    	
	    		posicao+=1;
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
	    		imagem_atual++;
	    	}

		    file.close(); //and finally close it:
		  	}

		  	else cout << "Unable to open file";

	}

int main(int argc, char const *argv[])
{

  	std::string data_path_dir  =  std::string(argv[1]);
  	CvKNearest knn_model;
		
		for(int n_train = 1; n_train <6;n_train++){
			std::vector<int> labels; //vetor que salva as classificações das imagens
			std::vector<Mat> images; //vetor que salva as imagens
			std::string data_path = data_path_dir+ "/data_batch_" + std::to_string(n_train) + ".bin";
			//"C:/Users/MC/Documents/2o semestre 2017/sistemas_inteligentes/ep_2/cifar-10-batches-bin"
			
			ler_bin(data_path, labels, images);
			int numberOfSamples = images.size()-1; //Número de amostras do arquivo de entrada

			int imgArea = images[0].rows*images[0].cols;

	
			Mat outputs(numberOfSamples,1,CV_32FC1,true); //Construtor da matriz de saídas para o treino
			Mat training(numberOfSamples,imgArea,CV_32FC1); //Construtor da matriz que conterá o dados de treino
		
			for(int contador=0; contador<numberOfSamples; contador++){
				int ii = 0; // Current column in training_mat

				outputs.at<float>(contador,0) = labels[contador];
			
				for (int l = 0; l<images[0].rows; l++) {
		    		for (int c = 0; c < images[0].cols; c++) {
		   	   			training.at<float>(contador,ii) = images[contador].at<float>(l,c);
		   	   			ii++;
		   			}
				}
			}

			//treino do modelo com os dados

			if(n_train ==1) knn_model.train(training, outputs, 0, false, 32, false);
			else knn_model.train(training, outputs, 0, false, 32, true);
		}		

	
}