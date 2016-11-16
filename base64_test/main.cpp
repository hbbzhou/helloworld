

#include <vector>
#include "base64.h"

struct IntAndInt2
{
	IntAndInt2()	{
		key = 0;
		value =0;
	}
	int key;
	int value;
};

void base64_test()
{
	const int len_ = 10;
	std::vector< IntAndInt2  > vec_0;
	for ( int i = 0; i < len_ ; ++i ){

		int nBattleAbility = rand() %1000;
		IntAndInt2 t_pair;
		t_pair.key = i;
		t_pair.value = nBattleAbility;
		vec_0.push_back(t_pair);
	}


	std::string encoded_str = base64_encode( (unsigned char const*)(&(vec_0[0])) , sizeof(IntAndInt2) * len_);
	std::string decoded = base64_decode(encoded_str);
	if(decoded.empty() ){
		return ;
	}

	const IntAndInt2 * p_obj = (const IntAndInt2 *)decoded.data();
	const int len_1 = decoded.size() / sizeof(IntAndInt2);
	std::vector< IntAndInt2  > vec_1;
	for(int i = 0 ; i < len_1 ; ++i){
		vec_1.push_back( *p_obj );
		++p_obj;
	}
}

int _tmain()
{
	base64_test();
	system("pause");
	return 0;
}















