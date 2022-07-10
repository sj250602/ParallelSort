#include "psort.h"
#include <omp.h>
#include <bits/stdc++.h>

uint32_t parti(uint32_t *arr,uint32_t l,uint32_t r){
	uint32_t p = arr[r],j = (l-1);
    for (uint32_t k=l;k<=r-1;k++){
		if(arr[k]<p){
			j++;
			uint32_t temp = arr[j];
			arr[j] = arr[k];
			arr[k] =  temp;
		}
	}	
	uint32_t temp = arr[j+1];
	arr[j+1] = arr[r];
	arr[r] =  temp;
	
	return (j+1);
}

void merge_sort(uint32_t *arr,uint32_t l,uint32_t r){
	if(l<r)
	{
		uint32_t mid = parti(arr,l,r);
		merge_sort(arr,l,mid-1);
		merge_sort(arr,mid+1,r);
	}
}

void ParallelSort(uint32_t *data, uint32_t n, int p)
{
    // Entry point to your sorting implementation.
    // Sorted array should be present at location pointed to by data.
	if(p==1){
		merge_sort(data,0,n-1);
	}else{
		//Procedure step 1
		
		uint32_t **arr = new uint32_t*[p];
		uint32_t  i;
		uint32_t x = 0;
		for(i=0;i<(n%p);i++){
			arr[i] = new uint32_t[n/p+1];
			for(uint32_t j = 0;j<(n/p+1);j++){
				arr[i][j] = data[x++];
			}
		}
		for(i = n%p;i<uint32_t(p);i++){
			arr[i] = new uint32_t[n/p];
			for(uint32_t j = 0;j<(n/p);j++){
				arr[i][j] = data[x++];
			}
		}
		
		// Procedure step 2
		uint32_t m = uint32_t(p)*uint32_t(p);
		uint32_t* R =  new uint32_t[m];
		x = 0;
		for(uint32_t j=0;j<uint32_t(p);j++){
			for(uint32_t k=0;k<uint32_t(p);k++){
				R[x++] = arr[j][k];
			}
		}
		std::sort(R,R+m);
		
		// for(uint32_t l=0;l<p*p;l++){
			// std::cout<<R[l]<<" ";
		// }
		// std::cout<<"\n";
		
		//Procedure step 3
		
		uint32_t *S = new uint32_t[p+1];
		S[0] = 0;
		for(uint32_t l=1;l<uint32_t(p);l++){
			S[l] = R[(l)*p];
		}
		S[p] = 4294967295;
		// for(uint32_t l=0;l<uint32_t(p-1);l++){
			// std::cout<<S[l]<<" ";
		// }
		// std::cout<<"\n";
		
		//Procedure step 4
		
		uint32_t **B = new uint32_t*[p];
		for(uint32_t l=0;l<uint32_t(p);l++){
			B[l] = new uint32_t[n];
		}
		
		uint32_t *size_arr = new uint32_t[p];
		
		for(uint32_t k=0;k<uint32_t(p);k++){
			#pragma omp task firstprivate(k)
			{
				uint32_t total_size = 0;
				uint32_t start;
				uint32_t end;
				if(k==0){
					start = S[0];
					end = S[1];
				}else if(k==uint32_t(p-1)){
					start = S[p-1];
					end = S[p];
				}else{
					start = S[k];
					end = S[k+1];
				}
				for(uint32_t l =0;l<n;l++){
					if(data[l]>start && data[l]<=end ){
						B[k][total_size] = data[l];
						total_size++;
					}
				}
				size_arr[k] = total_size;
			}
		}
		#pragma omp taskwait
		
		//Procedure step 5
		
		// for(uint32_t l=0;l<uint32_t(p);l++){
			// std::cout<<size_arr[l]<<" ";
		// }
		
		uint32_t Threshold = (2*n)/p;
		for(int k=0;k<p;k++){
			//size of array siz;
			uint32_t siz = size_arr[k];
			if(siz< Threshold){				
				std::sort(B[k],B[k]+siz);
			}else{
				ParallelSort(B[k],siz,p);
			}
		}
		
		// //Procedure step 6
		
		x = 0;
		for(uint32_t l=0;l<uint32_t(p);l++){
			uint32_t siz = size_arr[l];
			uint32_t col = 0;
			while(col!=siz){
				data[x++] = B[l][col++];
			}
		}
	}
}