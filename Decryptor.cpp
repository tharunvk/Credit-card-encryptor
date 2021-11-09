#include<iostream>
#include<string>
#include<math.h>
using namespace std;

int tweak;
string lcs(string key_chunk, int num) {
	string shifted = "";
	shifted += key_chunk.substr(num, key_chunk.size() - num);
	shifted += key_chunk.substr(0, num);
	return shifted;
}

string dec2bin(int ccn) {
	string bin;
	while (ccn != 0) {
		bin = (ccn % 2 == 0 ? "0" : "1") + bin;
		ccn = ccn / 2;
	}
	while (bin.length() < 4) {
		bin = "0" + bin;
	}
	return bin;
}

int bin2dec(string bin) {
	int dec = 0;
	int counter = 0;
	int size = bin.length();
	for (int i = size - 1; i >= 0; i--)
	{
		if (bin[i] == '1') {
			dec += pow(2, counter);
		}
		counter++;
	}
	return dec;
}


string XOR(string lhs, string rhs) {
	string xor_res = "";
	int size = rhs.size();
	for (int i = 0; i < size; i++) {
		if (lhs[i] != rhs[i]) {
			xor_res += "1";
		}
		else {
			xor_res += "0";
		}
	}
	return xor_res;
}

int get_six_digits(string ccn) {
	char temp[7];
	int k = 0;
	for (int i = 6; i < 12; i++) {
		temp[k++] = ccn[i];
	}
	temp[k] = '\0';
	return stoi(temp);
}
void subkeys_gen(string key, string* subkeys) {
	int pc1[56] = {
	57,49,41,33,25,17,9,
	1,58,50,42,34,26,18,
	10,2,59,51,43,35,27,
	19,11,3,60,52,44,36,
	63,55,47,39,31,23,15,
	7,62,54,46,38,30,22,
	14,6,61,53,45,37,29,
	21,13,5,28,20,12,4
	};

	int pc2[48] = {
	14,17,11,24,1,5,
	3,28,15,6,21,10,
	23,19,12,4,26,8,
	16,7,27,20,13,2,
	41,52,31,37,47,55,
	30,40,51,45,33,48,
	44,49,39,56,34,53,
	46,42,50,36,29,32
	};

	string pc1_string = "";
	for (int i = 0; i < 56; i++) {      //Reducing 64 bits to 56 using pc1
		pc1_string += key[pc1[i] - 1];
	}

	string lhs = pc1_string.substr(0, 28);   // Splitting key into 2 halves
	string rhs = pc1_string.substr(28, 28);

	//Performing LCS
	for (int i = 0; i < 16; i++) {
		// LCS shift by 1 for rounds 1,2,9,16
		if (i == 0 | i == 1 | i == 8 | i == 15) {
			lhs = lcs(lhs, 1);
			rhs = lcs(rhs, 1);
		}
		//LCS shift by two for other rounds
		else {
			lhs = lcs(lhs, 2);
			rhs = lcs(rhs, 2);
		}

		string shifted_key = lhs + rhs;
		string final_key = "";
		//Reducing to 48 bits using pc2
		for (int j = 0; j < 48; j++) {
			final_key += shifted_key[pc2[j] - 1];
		}

		subkeys[i] = final_key;
	}
}
int decryption_func(int num_to_decrypt, string * subkeys, int flag){
  int expansion_table[48] = {    //expansion table
  32,1,2,3,4,5,4,5,
  6,7,8,9,8,9,10,11,
  12,13,12,13,14,15,16,17,
  16,17,18,19,20,21,20,21,
  22,23,24,25,24,25,26,27,
  28,29,28,29,30,31,32,1
	};

  int sbox[8][4][16] =     //sboxes
	{ {
			14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
			0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
			4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
			15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
		},
		{
			15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
			3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
			0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
			13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
		},
		{
			10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
			13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
			13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
			1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
		},
		{
			7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
			13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
			10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
			3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
		},
		{
			2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
			14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
			4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
			11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
		},
		{
			12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
			10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
			9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
			4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
		},
		{
			4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
			13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
			1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
			6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
		},
		{
			13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
			1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
			7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
			2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
		} };
    
    //tweak added only if encryption
    if(flag==0){               
      num_to_decrypt = num_to_decrypt + tweak;
       if(num_to_decrypt>=1000000){
    	num_to_decrypt= num_to_decrypt - 1000000;
       }
    }
    string bits_to_decrypt = dec2bin(num_to_decrypt);


    //adding padding bits
    int pad = 20 - bits_to_decrypt.size();
	string strpad = "";
	for (int i = 0; i < pad; ++i)
	{
		strpad += '0';
	}
	bits_to_decrypt = strpad + bits_to_decrypt;

	string lhs, rhs;

	lhs = bits_to_decrypt.substr(0, 10);
	rhs = bits_to_decrypt.substr(10, 10);

	string zero_pad = "0000000000000000000000";

	for (int i = 0; i < 16; i++) {
		// EXpanding rhs

		string zero_pad_rhs = zero_pad + rhs;
		string expanded_rhs = "", xored_rhs = "";
		for (int j = 0; j < 48; j++) {
			expanded_rhs += zero_pad_rhs[expansion_table[j] - 1];
		}
		//XOR with key
		xored_rhs = XOR(expanded_rhs, subkeys[i]);


		// The s-box magic with FPE
		string rem_bits = "";
		while (xored_rhs.size() > 6) {

			string res = "";
			string temp = "";
			temp += xored_rhs.substr(xored_rhs.size() - 2, xored_rhs.size() % 6);
			rem_bits = temp + rem_bits;
			xored_rhs = xored_rhs.substr(0, xored_rhs.size() - temp.size());
			for (int j = 0; j < xored_rhs.size() / 6; j++) {

				string row = xored_rhs.substr(j * 6, 1) + xored_rhs.substr(j * 6 + 5, 1);
				int sbox_row = bin2dec(row);
				string col = xored_rhs.substr(j * 6 + 1, 1) + xored_rhs.substr(j * 6 + 2, 1) + xored_rhs.substr(j * 6 + 3, 1) + xored_rhs.substr(j * 6 + 4, 1);
				int sbox_col = bin2dec(col);
				int sbox_val = sbox[j][sbox_row][sbox_col];
				res += dec2bin(sbox_val);


			}
			xored_rhs = "";
			xored_rhs += res;

		}
		xored_rhs += rem_bits;
		string lr_xor = XOR(xored_rhs, lhs);

        //swapping lhs and rhs
		string temp = rhs;
		rhs = lr_xor;
		lhs = temp;		
	}
    string plaintext = rhs + lhs;
    return bin2dec(plaintext);

}
int main() {
	string ccn;
	cout<<" Enter Encrypted Credit card numer : ";   //Enter encrypted ccn
	cin>>ccn;

  if(ccn.size()<16){
    cout<<"Please enter valid 16 digit number"<<endl;
    exit(0);
  }
	
	int num_to_decrypt = get_six_digits(ccn);
	cout<<"Num to decrypt :"<<num_to_decrypt<<endl;
    tweak = stoi(ccn.substr(0,2)+ccn.substr(ccn.size()-4,4));    //getting middle 6 digits

	string key = "0110011001010100011010100101011101101110010110100111001000110100";
	string subkeys[16];
	subkeys_gen(key, subkeys);            //generating subkeys

    int k=0;
    string reversed_subkeys[16];     // reversing subkeys for decryption
	for (int i = 15; i >= 0; i--)
	{
		reversed_subkeys[k++] = subkeys[i];
	}

	

	do{
		num_to_decrypt = decryption_func(num_to_decrypt,reversed_subkeys,1);
		 
	}while(num_to_decrypt>=1000000);

    num_to_decrypt=num_to_decrypt-tweak;
    if(num_to_decrypt<0){
    	num_to_decrypt+=1000000;
    }
	cout<<"Decrypted number : "<<num_to_decrypt<<endl;

	string final_string = ccn.substr(0,6)+to_string(num_to_decrypt)+ccn.substr(ccn.size()-4,4);
    cout<<"Decrypted credit card number :"<<final_string<<endl;
    int getch;
    cin>>getch;
	return 0;
}

