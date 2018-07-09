/*
	AI Assignment 3
	Parkhi Mohan - 201601061
	J Sowmya Vasuki - 201601035
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void loadTrainData(double*** trainData, char trainF[], int rows, int cols, int k){
	int i, j, perFoldRows=ceil((rows*1.0)/k);
	// 3 dimensional memory allocation
	for(i=0; i<k; i++){
		trainData[i] = (double **)malloc(perFoldRows*sizeof(double *));
		for(j=0; j<perFoldRows; j++) trainData[i][j] = (double *)malloc(cols*sizeof(double));
	}
	// inputing values into trainData
	FILE *fp = fopen(trainF, "r");
	int ch, x=0, y=0, z=0, iterater=0;
	char data[10];
	while((ch=getc(fp)) != EOF){
		if(ch=='\n' || ch==' '){
			data[iterater] = '\0';
			iterater=0;
			trainData[x][y][z] = atoi(data);
			z++;
			if(z==cols){
				z=0;
				x++;
				if(x==k){
					x=0;
					y++;
					y=y%perFoldRows;
				}
			}
		}
		else{
			data[iterater] = ch;
			iterater++;
		}
	}
	while(x!=0){
		for(i=0; i<cols; i++) trainData[x][y][i] = -1.0;
		x++;
		x=x%k;
	}
	/*Printing and checking
	int count=0;
	for(i=0; i<k; i++){
		for(j=0; j<perFoldRows; j++){
			for(ch=0; ch<cols; ch++){
				count++;
				printf("%lf ", trainData[i][j][ch]);
			}
			printf("\n");
		}
	}
	printf("%d done\n", count);*/
}

void loadTestData(double **testData, char testF[], int rows, int cols){
	int i;
	// 2 dimensional memory allocation
	for(i=0; i<rows; i++) testData[i] = (double *)malloc(cols*sizeof(double ));
	// inputing values into trainData
	FILE *fp = fopen(testF, "r");
	int ch, x=0, y=0, iterater=0;
	char data[10];
	while((ch=getc(fp)) != EOF){
		if(ch=='\n' || ch==' '){
			data[iterater] = '\0';
			iterater=0;
			testData[x][y] = atoi(data);
			y++;
			if(y==cols){
				y=0;
				x++;
				x=x%rows;
			}
		}
		else{
			data[iterater] = ch;
			iterater++;
		}
	}
	/*	Printing and checking
	int i, j, count=0;
	for(i=0; i<rows; i++){
		for(ch=0; ch<cols; ch++){
			count++;
			printf("%lf ", testData[i][ch]);
		}
		printf("\n");
	}
	printf("%d done\n", count);*/
}

void sortKNN(double** kNN, int len){
	int i, j;
	double temp1, temp2;
	for(i=0; i<len; i++){
		for(j=i+1; j<len; j++){
			if(kNN[i][0] > kNN[j][0]){
				temp1 = kNN[i][0];
				temp2 = kNN[i][1];
				kNN[i][0] = kNN[j][0];
				kNN[i][1] = kNN[j][1];
				kNN[j][0] = temp1;
				kNN[j][1] = temp2;
			}
		}
	}
}

double euclideanDistance(double* v1, double* v2, int len){
	int i;
	double dist;
	for(i=0; i<len; i++) dist += (v1[i]-v2[i])*(v1[i]-v2[i]);
	dist = sqrt(dist);
	return dist;
}

int getLabel(double** kNN, int kOpt, int classes){
	int i, labelArr[10], temp=0, label;
	for(i=0; i<classes; i++) labelArr[i]=0;
	for(i=0; i<kOpt; i++) labelArr[(int)kNN[i][1]] += 1;
	for(i=0; i<classes; i++){
		if(labelArr[i] > temp){
			temp=labelArr[i];
			label=i;
		}
	}
	return label;
}

int predictor(double*** trainData, double* testData, int k, int kOpt, int rows, int classes, int cols){
	int i, j, p=0, label;
	// declaring kNN matrix: (euclideanDistance(double), label(double))
	double** kNN = (double **)malloc((kOpt+1)*sizeof(double *));
	for(i=0; i<=kOpt; i++) kNN[i] = (double *)malloc(2*sizeof(double));
	// initialising kNN to (-1.0, -1.0)
	for(i=0; i<=kOpt; i++){
		kNN[i][0] = -1.0;
		kNN[i][1] = -1.0;
	}
	// calculate k nearest neighbours for testData instance
	double dist;
	for(i=0; i<k; i++){
		for(j=0; j<rows; j++){
			if(trainData[i][j][cols-1] != -1.0){
				// finding euclidean distance
				dist = euclideanDistance(testData, trainData[i][j], cols-1);
				// finding k nearest neighbours on training set
				if(i==0 && j<=kOpt){
					kNN[p][0] = dist;
					kNN[p][1] = trainData[i][j][cols-1];
					p++;
					if(p==(kOpt+1)) sortKNN(kNN, kOpt+1);
				}
				else{
					if(dist < kNN[kOpt][0]){
						kNN[kOpt][0] = dist;
						kNN[kOpt][1] = trainData[i][j][cols-1];
						sortKNN(kNN, kOpt+1);
					}
				}
			}
		}
	}
	label = getLabel(kNN, kOpt, classes);
	return label;
}
int predictor1(double*** trainData,int j1,int k, int kOpt, int rows, int classes, int cols){
	int i, j, p=0, label;
	// declaring kNN matrix: (euclideanDistance(double), label(double))
	double** kNN = (double **)malloc((kOpt+1)*sizeof(double *));
	for(i=0; i<=kOpt; i++) kNN[i] = (double *)malloc(2*sizeof(double));
	// initialising kNN to (-1.0, -1.0)
	for(i=0; i<=kOpt; i++){
		kNN[i][0] = -1.0;
		kNN[i][1] = -1.0;
	}
	// calculate kOpt nearest neighbours for testData instance
	double dist;
	for(i=0; i<k; i++){
		if(i!=j1){
			for(j=0; j<rows; j++){
				if(trainData[i][j][cols-1] != -1.0){
					// finding euclidean distance
					dist = euclideanDistance(trainData[j1][j], trainData[i][j], cols-1);
					// finding k nearest neighbours on training set
					if(i==0 && j<=kOpt){
						kNN[p][0] = dist;
						kNN[p][1] = trainData[i][j][cols-1];
						p++;
						if(p==(kOpt+1)) sortKNN(kNN, kOpt+1);
					}
					else{
						if(dist < kNN[kOpt][0]){
							kNN[kOpt][0] = dist;
							kNN[kOpt][1] = trainData[i][j][cols-1];
							sortKNN(kNN, kOpt+1);
						}
					}
				}
			}
		}
	}
	label = getLabel(kNN, kOpt, classes);
	return label;
}
int min(float mean[]){
	int i;
	float x=100000000000000.0;
	for(i=0;i<3;i++){
		if(x>mean[i]) x=mean[i];
	}
	return ceil(x);
}
/*void plotGraph(float mean[]){
	char * commandsForGnuplot[] = {"set title \"CROSS ENTROPY WITH DELTA\"", "set xlabel \"NODES IN HIDDEN LAYER\"" , "set ylabel \"ACCURACY\"", "plot 'data.temp' with linespoints title 'ACCURACY'"};
    FILE * temp = fopen("data.temp", "w");
    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
    int i, arr[25];
    for(i=0; i<25; i++) arr[i] = i+1;
    for (i=0; i < 6 ; i++) fprintf(temp, "%d %lf \n", arr[i], mean[i]); //Write the data to a temporary file

    for (i=0; i < 4 ; i++) fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
}*/
void plotGraph(float mean[]){
	char * commandsForGnuplot[] = {"set title \"k-Nearest Neighbour Classifier using 3-fold Classification\"", "set xlabel \"k values\"" , "set ylabel \"Average Error Rate\"", "plot 'data.temp' with linespoints title 'Average Error Rate'"};
    FILE * temp = fopen("data.temp", "w");
    FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
    int i;
    for (i=0; i < 25 ; i++) fprintf(temp, "%d %lf \n", i+1, mean[i]); //Write the data to a temporary file

    for (i=0; i < 4 ; i++) fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
}


int KNNC1(double*** trainData, double** testData, int k, int rows, int cols, int perFoldRows, int classes){
	int i, label, correct=0, incorrect=0,k1=0,x=0, kOpt;
	float accuracy,arr[100],error,mean[100],sum;
	for(kOpt=1;kOpt<=25;kOpt++){
		k1=0;
		sum=0.0;
		for (int s=0;s<3; s++){
			correct=0;
			incorrect=0;
			for(i=0; i<rows; i++){
				if(testData[i][cols-1] != -1.0){
					label = predictor1(trainData, s, k, kOpt, perFoldRows, classes, cols);
					if((int)testData[i][cols-1] == label) correct+=1;
					else incorrect+=1;
				}
			}
			accuracy = (correct*1.0)/(correct+incorrect);
			error=1-accuracy;
			arr[k1]=error;
			k1++;
		}
		for(int p=0;p<k1;p++) sum+=arr[p];
		mean[x]=sum/3;
		//printf("Value of k: %d\tError found: %lf\n", kOpt, mean[x]);
		x++;
	}
  int u=min(mean);	
  printf("Optimum value of k found is: %d\n", u);
  plotGraph(mean);
  return u;
}

double KNNC(double*** trainData, double** testData, int k, int kOpt, int rows, int cols, int perFoldRows, int classes){
	int i, label, correct=0, incorrect=0;
	float accuracy;
	for(i=0; i<rows; i++){
		if(testData[i][cols-1] != -1.0){
			label = predictor(trainData, testData[i], k, kOpt, perFoldRows, classes, cols);
			if((int)testData[i][cols-1] == label) correct+=1;
			else incorrect+=1;
		}
	}
	accuracy = ((correct*1.0)*100.0)/(correct+incorrect);
	return accuracy;
}

int main(){
	int k=3, trainFR=6670, trainFC=193, testFR=3333, testFC=193;
	double ***trainData=(double ***)malloc((k)*sizeof(double **)), **testData=(double **)malloc((testFR)*sizeof(double *));
	char trainF[]="/home/parkhi/Desktop/pp_tra.dat", testF[]="/home/parkhi/Desktop/pp_tes.dat";
	
	// loading training and testing datakOp
	printf("Loading train data\n");
	loadTrainData(trainData, trainF, trainFR, trainFC, k);
	printf("Train data loaded\n");
	printf("Loading test data\n");
	loadTestData(testData, testF, testFR, testFC);
	printf("Test data loaded\n");
	
	// testing KNNC on testData
	int perFoldRows=ceil((trainFR*1.0)/k), classes=10,Ko;
	Ko=KNNC1(trainData, testData, k, testFR, testFC, perFoldRows, classes);
	//printf("%d\n",Ko);
	double accuracy, error;
	printf("Applying K-Nearest-Neighbours classifier to find optimum k\n");
	accuracy = KNNC(trainData, testData, k, Ko, testFR, testFC, perFoldRows, classes);
	error = 100.0 - accuracy;
	printf("Accuracy: %f\n",accuracy);
    printf("Error Rate: %f\n",error);
    
    free(trainData);
    free(testData);
	return 0;
}
