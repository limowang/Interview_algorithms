//寻找两个正序数组的中位数

#include <vector>
#include <iostream>

using namespace std;

//两个长度相等的有序数组中查找中位数
int pro(vector<int> nums1,vector<int> nums2)
{
    int start1 = 0;
    int end1 = nums1.size() - 1;
    int start2 = 0;
    int end2 = nums2.size() - 1;
    int mid1 = 0;
    int mid2 = 0;

    while(start1 < end1)
    {
        mid1 = (start1 + end1) / 2;
        mid2 = (start2 + end2) / 2;
        int len = start1 - end1 + 1;

        if(nums1[mid1] > nums2[mid2])
        {
            if(len % 2 == 0)
            {
                end1 = mid1;
                start2 = mid2 + 1;
            }else
            {
                end1 = mid1;
                start2 = mid2;
            }
        }else if(nums1[mid1] < nums2[mid2])
        {
            if(len % 2 == 0)
            {
                end2 = mid2;
                start1 = mid1 + 1;
            }else
            {
                end2 = mid2;
                start1 = mid1;
            }
        }else
        {
            return nums1[mid1];
        }
    }

    return min(nums1[start1],nums2[start2]);
}

//两个长度不等的数组找中位数
int proNotEq(vector<int>& nums1,vector<int>& nums2)
{
    vector<int> sA = nums1.size() < nums2.size() ? nums1 : nums2;


    vector<int> lA = sA == nums1 ? nums2 : nums1;
    int totalLen = sA.size() + lA.size();

    if(lA[totalLen / 2 - sA.size() - 1] >= sA[sA.size() - 1])
        return sA[sA.size() - 1];
    else
    {
        vector<int> tmp1 = sA;
        for(auto& it : tmp1)
            cout << it << endl;
        vector<int> tmp2(lA.begin() + totalLen / 2 - sA.size(),lA.begin() + totalLen / 2);
        for(auto& it : tmp2)
            cout << it << endl;
        int res = pro(tmp1,tmp2);
        cout << res << endl;
        return res;
    }
}

int main()
{
    vector<int> nums1 = {1,3,5};
    vector<int> nums2 = {4,5,6,10,11,12};

    cout << pro({1,3,5},{5,6,10}) << endl;

    cout << proNotEq(nums1,nums2) << endl;
    system("pause");
    return 0;
}

