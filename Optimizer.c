/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2017                              *
 *  Author: Ulrich Kremer                    *
 *  Student Version                          *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"
typedef struct Node {
    int reg;
    int address;
    struct Node *next;
}Regi;

int compareAddress(Regi* whole,int address){
    while(whole!=NULL){
        if(whole->address==address){
            return 1;
        }
        whole=whole->next;
    }
    return 0;
}

int compareReg(Regi* whole,int reg){
    while(whole!=NULL){
        if(whole->reg==reg){
            return 1;
        }
        
        
        whole=whole->next;
    }
    return 0;
}

Instruction *freeReg(Instruction *ptr){
    
    Instruction *tmp=ptr->next;
    Instruction *front=ptr->prev;
    if (tmp) {
        tmp->prev=front;
    }
    front->next=tmp;
free(ptr);
    return front;
}


Regi* CheckRegist(Regi* reg){
    if(reg==NULL){
        return NULL;
    }
    reg->next=CheckRegist(reg->next);
    if(reg->reg==0){
        if(reg->address==-1){
            Regi* ptr=reg->next;
            free(reg);
            return ptr;
        }
    }
    return reg;
}

int main()
{
    Instruction *head;
    head = ReadInstructionList(stdin);
    if (!head) {
        WARNING("No instructions\n");
        exit(EXIT_FAILURE);
    }
    /* YOUR CODE GOES HERE */
    Regi *RegHead=NULL;
    Regi *ptr=NULL;
    Regi *tmp=NULL;//creat head
    Instruction *rear=head;
    Instruction *new=NULL;
  rear=LastInstruction(rear);
    while (rear!=head) {//everything that didnot output or using by the output is useless and it will left head which is loadI 1014=>0
        if (rear->opcode==OUTPUTAI) {
            if (ptr==NULL) {
                RegHead=(Regi*)malloc(sizeof(Regi));
                RegHead->address=rear->field2;
                RegHead->reg=0;
                ptr=RegHead;
                ptr->next=NULL;
            }else{
                if(compareAddress(RegHead,rear->field2)==0){
                    tmp=(Regi*)malloc(sizeof(Regi));
                    tmp->address= rear->field2;
                    tmp->reg=0;
                    tmp->next=ptr;
                    ptr=tmp;
                    RegHead=ptr;
                }
            }
            if(!new){
                new=rear;
                //new->next=NULL;
            }else{
                new->prev=rear;
                rear->next=new;
                new=rear;
            }
            
        }else if(RegHead==NULL){
            
            
            
            rear=freeReg(rear);
            //rear=rear->prev;
            continue;
        }else{
            RegHead=CheckRegist(RegHead);
            ptr=RegHead;
           	tmp=RegHead;
            while (ptr!=NULL) {
                if (rear->opcode==STOREAI) {
                    if((ptr->address)==rear->field3){
                        ptr->address=-1;//matched
                        if(compareReg(RegHead,rear->field1)==0){
                            Regi* newAr=(Regi*)malloc(sizeof(Regi));
                            newAr->reg=rear->field1;//register cannot be 0 but address could be 0
                            newAr->address=-1;
                            if(tmp==ptr){
                                newAr->next=tmp;
                                RegHead=newAr;
                            }else{
                                tmp->next=newAr;
                                newAr->next=ptr;
                            }
                        }
                        new->prev=rear;
                        rear->next=new;
                        new=rear;
                        
                        break;
                    }
                }else if(rear->opcode==LOADAI){//field3
                    if(ptr->reg==rear->field3){//will not show match to 0, because register 0 will never be used
                        
                        ptr->reg=0;
                        if(compareAddress(RegHead,rear->field2)==0){
                            Regi* newAr=(Regi*)malloc(sizeof(Regi));
                            newAr->address=rear->field2;
                            //only when both is 0, it can free(address and register will not show at the same time)
                            newAr->reg=0;
                            if(tmp==ptr){
                                newAr->next=tmp;
                                RegHead=newAr;
                            }else{
                                tmp->next=newAr;
                                newAr->next=ptr;
                            }
                        }
                        new->prev=rear;
                        rear->next=new;
                        new=rear;
                        break;
                    }
                }else if((rear->opcode==ADD)||(rear->opcode==SUB)||(rear->opcode==MUL)||(rear->opcode==DIV)){//field 3
                    if(ptr->reg==rear->field3){//will not show match to 0, because register 0 will never be used
                        ptr->reg=0;
                        if(compareReg(RegHead,rear->field1)==0){
                            Regi* newAr=(Regi*)malloc(sizeof(Regi));
                            newAr->reg=rear->field1;
                            newAr->address=-1;
                            if(compareReg(RegHead,rear->field2)==0){
                                Regi* newA=(Regi*)malloc(sizeof(Regi));
                                newA->reg=rear->field2;
                                newA->address=-1;
                                newAr->next=newA;
                                //only when both is 0, it can free(address and register will not show at the same time)
                                if(tmp==ptr){
                                    newA->next=tmp;
                                    RegHead=newAr;
                                }else{
                                    tmp->next=newAr;
                                    newA->next=ptr;
                                }
                            }else{
                                if(tmp==ptr){
                                    newAr->next=tmp;
                                    RegHead=newAr;
                                }else{
                                    tmp->next=newAr;
                                    newAr->next=ptr;
                                }
                            }
                        }
                        new->prev=rear;
                        rear->next=new;
                        new=rear;
                        break;
                    }
                }else if(rear->opcode==LOADI){//field 2
                    if(ptr->reg==rear->field2){//will not show match to 0, because register 0 will never be used
                        //only when both is 0, it can free(address and register will not show at the same time)
                        ptr->reg=0;
                        new->prev=rear;
                        rear->next=new;
                        new=rear;
                        break;
                    }
                }
                tmp=ptr;
                ptr=ptr->next;
            }
            

        }
 if (rear!=new) {
                rear=freeReg(rear);
            }else{
        rear=rear->prev;
}
    }
    ptr=RegHead;
    while(RegHead){
        ptr=RegHead->next;
        free(RegHead);
        RegHead=ptr;
    }
    if(new){
        head->next=new;
        new->prev=head;
    }else{
        head->next=NULL;
    }
    if (head)
        PrintInstructionList(stdout, head);
    while(head){
new=head->next;
free(head);
head=new;
}
    return EXIT_SUCCESS;
}

