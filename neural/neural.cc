#include <iostream>

#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>

float **in_hidden;
float **hidden_out;

float e(float t, float o){
	return pow(t-o, 2)/2.0;
}

float de(float t, float o){
	return -(t-o);
}

float unit(float x){
	return 1/(1+exp(-x));
}

float dunit(float o){
//	float e_x = exp(-x);
//	return e_x/pow(1+e_x, 2);

	return o * (1-o); 
}

void output(std::vector<float> &v){
	std::cout << '[';
	for(unsigned int i = 0; i < v.size()-1; ++i){
		std::cout << v[i] << ',';
	}

	if(v.size() > 0)
		std::cout << v[v.size()-1];
	std::cout << "]\n";
}

float randomw(float min=-0.1, float max=0.1){
	return (rand()%1000)/1000.0 * (max-min) + min;
}

void setup(unsigned int innum, unsigned int hiddennum, unsigned int outnum){
	in_hidden = new float*[innum];
	for(unsigned int i = 0; i < innum; ++i){
		in_hidden[i] = new float[hiddennum];
		for(unsigned int j = 0; j < hiddennum; ++j){
			in_hidden[i][j] = randomw();
		}
	}

	hidden_out = new float*[hiddennum];
	for(unsigned int i = 0; i < hiddennum; ++i){
		hidden_out[i] = new float[outnum];
		for(unsigned int j = 0; j < outnum; ++j){
			hidden_out[i][j] = randomw();
		}
	}
}

void train(std::vector<float> &in, std::vector<float> &target, float eta=0.3){
	std::vector<float> hidden;
	std::vector<float> out;

	// calc each hidden value
	for(unsigned int i = 0; i < 3; ++i){
		hidden.push_back(0);
		for(unsigned int j = 0; j < in.size(); ++j){
			hidden[i] += in_hidden[j][i] * in[j];
		}
		hidden[i] = unit(hidden[i]);
	}

	// calc each output value
	for(unsigned int i = 0; i < 8; ++i){
		out.push_back(0);
		for(unsigned int j = 0; j < hidden.size(); ++j){
			out[i] += hidden_out[j][i] * hidden[j];
		}
		out[i] = unit(out[i]);
	}

	// gradient descent
	std::vector<float> output_delta;
	for(unsigned int i = 0; i < out.size(); ++i){
		float delta = de(target[i], out[i]) * dunit(out[i]);
		output_delta.push_back(delta);
	}

	std::vector<float> hidden_delta;
	for(unsigned int i = 0; i < hidden.size(); ++i){
		float delta = 0.0;
		for(unsigned int j = 0; j < out.size(); ++j){
			delta += output_delta[j] * hidden_out[i][j];
		}
		hidden_delta.push_back(delta * dunit(hidden[i]));
	}

	// update hidden_out
	for(unsigned int i = 0; i < hidden.size(); ++i){
		for(unsigned int j = 0; j < out.size(); ++j){
			hidden_out[i][j] += -eta * output_delta[j] * hidden[i];
		}
	}
	
	// update in_hidden
	for(unsigned int i = 0; i < in.size(); ++i){
		for(unsigned int j = 0; j < hidden.size(); ++j){
			in_hidden[i][j] += -eta * hidden_delta[j] * in[i];
		}
	}
}

std::vector<float> test(std::vector<float> &in){
	std::vector<float> hidden;
	std::vector<float> out;

	// calc each hidden value
	for(unsigned int i = 0; i < 3; ++i){
		hidden.push_back(0);
		for(unsigned int j = 0; j < in.size(); ++j){
			hidden[i] += in_hidden[j][i] * in[j];
		}
		hidden[i] = unit(hidden[i]);
	}

	output(hidden);

	// calc each output value
	for(unsigned int i = 0; i < 8; ++i){
		out.push_back(0);
		for(unsigned int j = 0; j < hidden.size(); ++j){
			out[i] += hidden_out[j][i] * hidden[j];
		}
		out[i] = unit(out[i]);
	}

	return out;
}


int main(){
	srand(time(NULL));
	setup(8, 3, 8);

	float in1[8] = {1, 0, 0, 0, 0, 0, 0, 0};
	float in2[8] = {0, 1, 0, 0, 0, 0, 0, 0};
	float in3[8] = {0, 0, 1, 0, 0, 0, 0, 0};
	float in4[8] = {0, 0, 0, 1, 0, 0, 0, 0};
	float in5[8] = {0, 0, 0, 0, 1, 0, 0, 0};
	float in6[8] = {0, 0, 0, 0, 0, 1, 0, 0};
	float in7[8] = {0, 0, 0, 0, 0, 0, 1, 0};
	float in8[8] = {0, 0, 0, 0, 0, 0, 0, 1};

	std::vector<float> vin1(in1, in1+8);
	std::vector<float> vin2(in2, in2+8);
	std::vector<float> vin3(in3, in3+8);
	std::vector<float> vin4(in4, in4+8);
	std::vector<float> vin5(in5, in5+8);
	std::vector<float> vin6(in6, in6+8);
	std::vector<float> vin7(in7, in7+8);
	std::vector<float> vin8(in8, in8+8);

	for(int i = 0; i < 10000; ++i){
		train(vin1, vin1);
		train(vin2, vin2);
		train(vin3, vin3);
		train(vin4, vin4);
		train(vin5, vin5);
		train(vin6, vin6);
		train(vin7, vin7);
		train(vin8, vin8);
	}

	test(vin1);
	test(vin2);
	test(vin3);
	test(vin4);
	test(vin5);
	test(vin6);
	test(vin7);
	test(vin8);
	return 0;
}
