/*
	AI Assignment 3
	Parkhi Mohan - 201601061
	J Sowmya Vasuki - 201601035
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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

double* posteriorProb(double*** trainData, int classes, double postProbArr[], int rows, int k, int trainFR){
	int i, j, ite;
	double *classprob = (double *)malloc(10*sizeof(double));
	for(i=0; i<k; i++){
		for(j=0; j<rows; j++){
			postProbArr[(int)trainData[i][j][192]]++;
		}
	}
	for(i=0; i<classes; i++) classprob[i] = postProbArr[i]/(trainFR*1.0);
	//for(i=0; i<classes; i++) printf("%lf \n", postProbArr[i]);
	return classprob;
}

double*** findingIntersection(int classes, int rows, int k, double*** trainData, int cols){
	int i, j, ite;
	// 3 dimensional memory allocation
	double ***intersection=(double ***)malloc(classes*sizeof(double **));
	for(i=0; i<classes; i++){
		intersection[i] = (double **)malloc(193*sizeof(double *));
		for(j=0; j<193; j++){
			intersection[i][j] = (double *)malloc(5*sizeof(double));
		}
	}
	for(i=0; i<k; i++){
		for(j=0; j<rows; j++){
			for(ite=0; ite<cols-1 && trainData[i][j][192]!=-1.0; ite++) intersection[(int)trainData[i][j][192]][ite][(int)trainData[i][j][ite]] += 1.0;
		}
	}	
	/*for(i=0; i<classes; i++){
		for(j=0; j<192; j++){
			for(ite=0; ite<5; ite++)
				printf("%lf\t", intersection[i][j][ite]);
			printf("\n");
		}
		printf("\n");
	}*/
	return intersection;
}

void interStep(double*** intersection, double* postProbArr){
	int i, j, k, ite;
	for(i=0;i<10;i++){
		for(j=0;j<192;j++){
			for(k=0;k<5;k++) intersection[i][j][k]/=postProbArr[i];
		}
	}
}

int maximumVal(double mean[]){
	int i, temp=0;
	double x=mean[0];
	for(i=0;i<10;i++){
		if(x<mean[i]){
			temp=i;
			x=mean[i];
		}
	}
	return temp;
}

double naiveBayesClassifier(double** testData, double*** intersection, double* classprob, int rows){
	int i, j, k, r, m, correct=0, incorrect=0;
	double temp=0.0,decision[10],label[3334], accuracy;
	for(r=0;r<rows;r++){
		for(i=0;i<10;i++){
			temp=0.0;
			for(j=0;j<192;j++){
				temp += log(intersection[i][j][(int)testData[r][j]]);
			}
			temp += log(classprob[i]);			
			decision[i] = temp;
		}
		label[r] = maximumVal(decision);
	}
	for (i = 0; i <rows; i++){
		if((int)label[i] == (int)testData[i][192]) correct += 1;
		else incorrect++;
	}
	accuracy = ((correct*1.0)*100.0)/(correct+incorrect);
	return accuracy;
	
}

int main(){
	int i, k=3, trainFR=6670, trainFC=193, testFR=3333, testFC=193, classes=10, perFoldRows = ceil((trainFR*1.0)/k);
	double postProbArr[10], *classprob, accuracy, error;
	for(i=0; i<classes; i++) postProbArr[i]=0.0;
	double ***trainData=(double ***)malloc((k)*sizeof(double **)), **testData=(double **)malloc((testFR)*sizeof(double *)), ***intersection; 
	//double ***intersection=(double ***)malloc((classes)*sizeof(double **));
	char trainF[]="/home/parkhi/Desktop/pp_tra.dat", testF[]="/home/parkhi/Desktop/pp_tes.dat";
	
	// loading training and testing datakOp
	printf("Loading train data\n");
	loadTrainData(trainData, trainF, trainFR, trainFC, k);
	printf("Train data loaded\n");
	printf("Loading test data\n");
	loadTestData(testData, testF, testFR, testFC);
	printf("Test data loaded\n");
	
	// calculating P(w)
	classprob = posteriorProb(trainData, classes, postProbArr, perFoldRows, k, trainFR);
	
	// finding intersection
	intersection = findingIntersection(classes, perFoldRows, k, trainData, trainFC);
	interStep(intersection,postProbArr);
	printf("Performing Naive Bayes Classification\n");
	accuracy = naiveBayesClassifier(testData, intersection, classprob, testFR);
	error = 100.0 - accuracy;
	printf("Accuracy: %lf\n",accuracy);
    printf("Error Rate: %lf\n",error);

    free(trainData);
    free(testData);
	return 0;
}
