#include<stdio.h>
int strings_not_equal(const char*, const char*);

void explode_bomb(){
    printf("\nBOOM!!!");
    printf("The bomb has blown up.");
    exit();
}

void phase_1(const char *input){
    if(strings_not_equal(input, "Border relations with Canada have never been better.")){
        explode_bomb();
    }
    else{
        return;
    }
}

void phase_2(const char* input){
    int array[6];
    read_six_numbers(input, array); // Reading six integer in sequence and storing them into the array
    if(array[0] != 1){
        explode_bomb();
    }
    for(int i = 1; i < 6; ++i){
        if(array[i-1] * 2 != array[i])  explode_bomb();
    }
}

void phase_3(const char* input){
    int selection, ans, sol;
    int argc = sscanf(input, "%d %d", selection, ans);
    if(argc != 2){
        explode_bomb();
    }
    switch(selection){
        case 0:
            sol = 207;
            break;
        case 1:
            sol = 311;
            break;
        case 2:
            sol = 707;
            break;
        case 3:
            sol = 256;
            break;
        case 4:
            sol = 389;
            break;
        case 5:
            sol = 206;
            break;
        case 6:
            sol = 682;
            break;
        case 7: 
            sol = 327;
            break;
        default:
            explode_bomb();
            break;
    }
    if(ans != sol)  explode_bomb();

}

int func4(int v1, int v2, int v3){
    int eax = v3 - v2;
    int ecx = ((unsigned int)eax)>>31;
    eax = (eax + ecx) >> 1;
    ecx = eax + v2;
    if(ecx > v1){
        eax = func4(v1, v2, ecx - 1);
        eax *= 2;
    }
    else{
        eax = 0;
        if(ecx < v1){
            eax = func4(v1, ecx+1, v3);
            eax = 2*eax + 1;
        }
    } 
    return eax;
}


void phase_4(const char* input){
    int v1, v2;
    int argc = sscanf(input, "%d %d", v1, v2);
    if(argc != 2 || (unsigned int)v1 > 14){
        explode_bomb();
    }

    int x = func4(v1, 0, 14);
    if(x != 0 || v2 != 0){
        explode_bomb();
    }
}


void phase_5(const char* input){
    char mapping_arr[] = "maduiersnfotvbyl"; 
    char buf[8];
    int len  = strlen(input);
    if(len != 6) explode_bomb();
    
    // Remapping the given string with mapping array.
    int p = 0;
    for(p = 0; p < 6; ++p){
        buf[p] = mapping_arr[input[p] & 0x0F];
    }
    buf[p] = '\0';

    if(strings_not_equal(buf, "flyers")){
        explode_bomb();
    }
}

typedef struct node{
    int data, index;
    node* next;    
}node;

void phase_6(const char* input){
    int array[6];
    read_six_numbers(input, array); // Reading six integer in sequence and storing them into the array

    // The elements in array should stay in range from 1 ~ 6. 
    // Further, any pair of number shouldn't be the same. (In short, the input array shall be the shuffle of 1 ~ 6) 
    for(int p1 = 0, p2 = 1; p2 <= 5; ++p1, ++p2){
        if((unsigned int)(array[p1] - 1) > 5)   explode_bomb();
        
        for(int index = p2; index <= 5; ++index){
            if(array[index] == array[index])   explode_bomb();
        }
    }

    // Reassign each element with 7 - itself.
    for(int index = 0; index < 6; ++index){
        int x = 7;
        x -= array[index];
        array[index] = x;
    }

    // Reordering a linked list by comparing its indices with our transformed input array.
    int index = 0, value;
    extern node* list;
    node* node_ptr;
    node* node_arr[6];
    for(; index < 6; ++index){
        if(array[index] <= 1){
            node_ptr = array;
            node_arr[index] = node_ptr;
            continue;
        }
        
        value = 1;
        node_ptr = array;
        do{
            node_ptr = node_ptr->next;
            value++;
        }while(array[index] != value);
        
        node_arr[index] = node_ptr;
    }

    /*
    int esi = 0, eax;
    extern node* list;
    node* node_ptr;
    node* node_arr[6];
    for(; esi < 6; ++esi){
        if(array[esi] <= 1){
            goto L2;
        }
        
        eax = 1;
        node_ptr = array;
    L1: 
        node_ptr = node_ptr->next;
        eax++;
        if(array[esi != eax])   goto L1;
        goto L3;
    L2: 
        node_ptr = array;
    L3:
        node_arr[esi] = node_ptr;
    }
    */

    // Reconstructing the linked list after reordering.
    node_ptr = node_arr[0];
    index = 1;
    while(1){
        node_ptr->next = node_arr[index];
        if(++index == 6)    break;
        node_ptr = node_ptr->next;
    }
    node_ptr->next = NULL;
    
    // After reordering and reconstructing, the list should be a decreasing order.
    int num = 5;
    node_ptr = list;
    while(num--){
        node *next = node_ptr->next;
        if(next->data > node_ptr->data) explode_bomb();
    }

}

typedef struct Tree_node{
    int data;
    Tree_node *left, *right;
}Tree_node;

int fun7(Tree_node *node, int b){
    if(!node) return -1;
    int eax;
    if(node->data > b){
        node = node->left;
        eax = fun7(node, b);
        eax *= 2;
    }
    else if(node->data == b){
        eax = 0;
    }
    else{
        node = node->right;
        eax = fun7 (node, b);
        eax = 2*eax + 1;
    }

    return eax;
}