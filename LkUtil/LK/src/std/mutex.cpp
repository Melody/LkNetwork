#include "stdafx.h"
#ifdef STD_MUTEX_
namespace std {
	void condition_variable::notify_all(){
		vector<native_handle_type> vec;
		mt.lock();
		LK::StructOfSize<sizeof(vector<native_handle_type>)> st = (LK::StructOfSize<sizeof(vector<native_handle_type>)>&)handles;
		(LK::StructOfSize<sizeof(vector<native_handle_type>)>&)handles = (LK::StructOfSize<sizeof(vector<native_handle_type>)>&)vec;
		((LK::StructOfSize<sizeof(vector<native_handle_type>)>&)vec) = st;
		mt.unlock();
		for (vector<native_handle_type>::iterator it = vec.begin(); it != vec.end(); ++it){
			*(bool*)*it = false;
		}
	}
}
#endif