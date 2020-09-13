#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

const char END_OF_TEST_DELIMITER = '#';
const char COLUMN_DELIMITER = ';';
const string FIXED_TITLE = "Version;N;BS;T;Exec time;GFlops;Coef.var.;Excecs count";
const int VECTOR_RESERVE = 16;

class ExecutionResults{
	private:
		vector<float> execTimes;
		vector<float> gFlops;
		string params;

		float calculateAverage(vector<float>& vec){
			float sum = 0;
			int n = vec.size();
			for(int i=0; i<n; i++)
				sum += vec[i];
			return sum/n;
		}
		float calculateVariance(vector<float>& vec){
			float mean = calculateAverage(vec);
			float summatory = 0;
			int n = vec.size();
			for(int i=0; i<n; i++)
				summatory += pow(vec[i] - mean, 2) / n;
			return summatory;
		}
		float calculateStandardDeviation(vector<float>& vec){
			return sqrt(calculateVariance(vec));
		}
		string vecToString(vector<float>& vec){
			string str = "";
			int n = vec.size();
			for(int i=0; i<n; i++){
				str+= to_string(vec[i]);
				if(i<n-1)
					str+= COLUMN_DELIMITER;
			}
			return str;
		}
		string float_to_string(float fl, int precision = 2){
			stringstream stream;
			stream << fixed << setprecision(precision) << fl;
			return stream.str();
		}

	public:
		ExecutionResults(){
			execTimes = vector<float>();
			execTimes.reserve(VECTOR_RESERVE);
			gFlops = vector<float>();
			gFlops.reserve(VECTOR_RESERVE);
		}
		void setParams(string params){
			this->params = params;
		}
		string getParams(){
			return this->params;
		}
		void addResult(float execTime, float GFlops){
			this->execTimes.push_back(execTime);
			this->gFlops.push_back(GFlops);
		}
		float getAvgExecTimes(){
			return this->calculateAverage(this->execTimes);
		}
		float getAvgGFlops(){
			return this->calculateAverage(this->gFlops);
		}
		int getExecsCount(){
			return this->execTimes.size();
		}
		float getCoeficientOfVariation(){
			float SD = this->calculateStandardDeviation(this->execTimes);
			float mean = this->calculateAverage(this->execTimes);
			return SD / mean;
		}
		string getSummaryInString(){
			return this->float_to_string(this->getAvgExecTimes(),2) + COLUMN_DELIMITER + this->float_to_string(this->getAvgGFlops(),3) + COLUMN_DELIMITER + this->float_to_string(this->getCoeficientOfVariation()*100,2) + COLUMN_DELIMITER + to_string(this->getExecsCount());
		}
		string getExecTimesInString(){
			return "Exec. times: "+this->vecToString(this->execTimes);
		}
		string getGFlopsInString(){
			return "GFlops: "+this->vecToString(this->gFlops);
		}
};

class CsvResultRegenerator{
	private:
		vector<ExecutionResults*> testsResults;
		
/*		void supressSemicolon(string& str){
			str[str.length()-1] = '\0'; //Se pisa con null el ultimo caracter del string (el que sería el punto y coma en el CSV)
		}*/
		void readLn(string& str){
			getline(cin, str);
	//		cout << "String= "+str << endl;
	//		supressSemicolon(str);
		}
		void writeLn(string str){
			cout << str <<  endl;
		}
		bool endOfTest(const string& str){
			return (str[0] == END_OF_TEST_DELIMITER);
		}
		void addTestResult(ExecutionResults* er){
			this->testsResults.push_back(er);
		}
		void parseResult(string rowStr, ExecutionResults* er){
			string execTimeStr, gFlopsStr;
			int pos = rowStr.find(COLUMN_DELIMITER);
			execTimeStr = rowStr.substr(0, pos);
			gFlopsStr = rowStr.substr(pos+1, rowStr.length()-pos);
	/*		cout << "execTimeStr= " << execTimeStr << endl;
			cout << "gFlopsStr= " << gFlopsStr << endl;*/
			er->addResult(stof(execTimeStr), stof(gFlopsStr));
		}
	
	public:
		void readCsv(){
			string inputStr, params;
			ExecutionResults* er;

			do{
				readLn(inputStr);
		/*		cout << "String= "+result << endl;
				exit(0);*/
			}while(!cin.eof() && (inputStr == "")); //Para limpiar todas las filas vacias que puedan venir al principio
		//	En este punto el inputStr tiene cargado el titulo 1 (descartable)
			while(!cin.eof() && (inputStr != "")){ //!cin.eof() esta al pedo aca y en el while interno.. no tiene efecto ni escribiendo en consola ni pasando un archivo.
				er = new ExecutionResults();
				readLn(params);
				readLn(inputStr); //Titulo 2 (descartable)
				readLn(inputStr); //Test result
				er->setParams(params);
				while( !cin.eof() && (inputStr != "") && !endOfTest(inputStr) ){
					this->parseResult(inputStr, er);
					readLn(inputStr); //Test result
				}
				this->testsResults.push_back(er);
				readLn(inputStr);
			}
		}
		void generateSumarizedCsv(){
			ExecutionResults* er;
			writeLn(FIXED_TITLE);
			for(int i=0; i<this->testsResults.size(); i++){
				er = this->testsResults[i];
				writeLn(er->getParams() + COLUMN_DELIMITER + er->getSummaryInString());
			//	writeLn(er->getExecTimesInString());
			//	writeLn(er->getGFlopsInString());
			//	writeLn("");
			}
		}
};

int main(){
	CsvResultRegenerator csvRegenerator;
	csvRegenerator.readCsv();
	csvRegenerator.generateSumarizedCsv();
	return 0;
}
