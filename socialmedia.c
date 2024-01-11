/**
 * 
 * Graphs &
 * Recursion
 * In
 * Toronto
 * 
 * (c) 2020 William Song, Mustafa Quraish
 * (c) 2021 Charles Xu
 **/

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024

#ifndef MAT_SIZE
#define MAT_SIZE 3//smallgraph
#endif

typedef struct user_struct
{
  char name[MAX_STR_LEN];
  struct friend_node_struct *friends;
  struct brand_node_struct *brands;
  bool visited;
  int dist;
} User;

typedef struct friend_node_struct
{
  User *user;
  struct friend_node_struct *next;
} FriendNode;

typedef struct brand_node_struct
{
  char brand_name[MAX_STR_LEN];
  struct brand_node_struct *next;
} BrandNode;

typedef struct Queue_node{
	User*user;
	struct Queue_node*next;
}Q_node;
typedef struct Queue_struct
{
	Q_node*front;
	Q_node*end;
}Queue;
	

	
	
FriendNode *allUsers = NULL;

int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Given the head to a FriendNode linked list, returns true if a
 * given user's name exists in the list. Returns false otherwise.
 */
bool in_friend_list(FriendNode *head, User *node)
{
  for (FriendNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->user->name, node->name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Given the head to a BrandNode linked list, returns true if a
 * given brand's name exists in the list. Returns false otherwise.
 */
bool in_brand_list(BrandNode *head, char *name)
{
  for (BrandNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->brand_name, name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Given the head to a FriendNode linked list, inserts a given user
 * into the linked list and returns the head of the new linked list.
 * The insertion is done in alphabetical order. If the user already
 * exists, no modifications are made and the list is returned as is.
 */
FriendNode *insert_into_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (in_friend_list(head, node))
  {
    printf("User already in list\n");
    return head;
  }

  FriendNode *fn = calloc(1, sizeof(FriendNode));
  fn->user = node;
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->user->name, node->name) > 0)
  {
    fn->next = head;
    return fn;
  }

  FriendNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

/**
 * Given the head to a BrandNode linked list, inserts a given brand into
 * the linked list and returns the head of the new linked list. The
 * insertion is done in alphabetical order. If the brand already exists,
 * no modifications are made and the list is returned as is.
 */
BrandNode *insert_into_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (in_brand_list(head, node))
  {
    printf("Brand already in list\n");
    return head;
  }

  BrandNode *fn = calloc(1, sizeof(BrandNode));
  strcpy(fn->brand_name, node);
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->brand_name, node) > 0)
  {
    fn->next = head;
    return fn;
  }

  BrandNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

/**
 * Given the head to a FriendNode linked list, removes a given user from
 * the linked list and returns the head of the new linked list. If the user
 * does not exist, no modifications are made and the list is returned as is.
 */
FriendNode *delete_from_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (!in_friend_list(head, node))
  {
    printf("User not in list\n");
    return head;
  }

  if (strcmp(head->user->name, node->name) == 0)
  {
    FriendNode *temp = head->next;
    free(head);
    return temp;
  }

  FriendNode *cur;
  for (cur = head; cur->next->user != node; cur = cur->next);

  FriendNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

/**
 * Given the head to a BrandNode linked list, removes a given brand from
 * the linked list and returns the head of the new linked list. If the brand
 * does not exist, no modifications are made and the list is returned as is.
 */
BrandNode *delete_from_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (!in_brand_list(head, node))
  {
    printf("Brand not in list\n");
    return head;
  }

  if (strcmp(head->brand_name, node) == 0)
  {
    BrandNode *temp = head->next;
    free(head);
    return temp;
  }

  BrandNode *cur;
  for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
    ;

  BrandNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

/**
 * Given a user, prints their name, friends, and liked brands.
 */
void print_user_data(User *user)
{
  printf("User name: %s\n", user->name);

  printf("Friends:\n");
  for (FriendNode *f = user->friends; f != NULL; f = f->next)
  {
    printf("   %s\n", f->user->name);
  }

  printf("Brands:\n");
  for (BrandNode *b = user->brands; b != NULL; b = b->next)
  {
    printf("   %s\n", b->brand_name);
  }
}

/**
 * Given a brand, returns the index of the brand inside the brand_names array.
 * If it doesn't exist in the array, return -1
 */
int get_brand_index(char *name)
{
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (strcmp(brand_names[i], name) == 0)
    {
      return i;
    }
  }

  printf("Brand '%s' not found\n", name);
  return -1; // Not found
}

/**
 * Given a brand, prints their name, index (inside the brand_names
 * array), and the names of other similar brands.
 */
void print_brand_data(char *brand_name)
{
  int idx = get_brand_index(brand_name);
  if (idx < 0)
  {
    printf("Brand '%s' not in the list.\n", brand_name);
    return;
  }

  printf("Brand name: %s\n", brand_name);
  printf("Brand idx: %d\n", idx);

  printf("Similar brands:\n");
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0)
    {
      printf("   %s\n", brand_names[i]);
    }
  }
}

/**
 * Read from a given file and populate a the brand list and brand matrix.
 **/
void populate_brand_matrix(char *file_name)
{
  // Read the file
  char buff[MAX_STR_LEN];
  FILE *f = fopen(file_name, "r");
  fscanf(f, "%s", buff);
  char *line = buff;
  // Load up the brand_names matrix
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (i == MAT_SIZE - 1)
    {
      strcpy(brand_names[i], line);
      break;
    }
    int index = strchr(line, ',') - line;
    strncpy(brand_names[i], line, index);
    line = strchr(line, ',') + sizeof(char);
  }
  // Load up the brand_adjacency_matrix
  for (int x = 0; x < MAT_SIZE; x++)
  {
    fscanf(f, "%s", buff);
    for (int y = 0; y < MAT_SIZE; y++)
    {
      int value = (int)buff[y * 2];
      if (value == 48)
      {
        value = 0;
      }
      else
      {
        value = 1;
      }
      brand_adjacency_matrix[x][y] = value;
    }
  }
}

/**
 * TODO: Complete this function
 * Creates and returns a user.
   Given a name, creates a new user on the platform and returns the created user. When the user is created, 
   they must also be inserted into the allUsers linked list.
   If a user already existed with the same name, do nothing and return NULL instead.
 */
User *create_user(char *name)
{
	User *node;
	node=(User*)calloc(1,sizeof(User)); // allocates space to store the name of the user
	strcpy(node->name,name);
	node->friends=NULL;
	node->brands=NULL;
	if(in_friend_list(allUsers,node)==false)
	{
		   allUsers=insert_into_friend_list(allUsers,node);
		   return node;
	}
	return NULL;
	
}
////////////////////////////////////////////////////////////////////////////////////////
// Helper for delete user 

 FriendNode*free_user_friend_list(FriendNode*head)
 {
	FriendNode*pt=NULL;//p
    FriendNode*tem=NULL;//q
    pt=head;
    while(pt!=NULL)
    {
	   tem=pt->next;
	   free(pt);
	   pt=tem;
    }
  return NULL;
 }
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * TODO: Complete this function
 * Deletes a given user.
 Removes a given user from the platform. The user must be removed from the allUsers linked list and the friend list of any users that they belong to.
 Return 0 if the user was successfully removed. 
 If the user does not exist, return -1 instead.
 
 */
int delete_user(User *user)
{ 


     user->friends=free_user_friend_list(user->friends);	
	 if ( in_friend_list(allUsers,user)==true)
	 {
	   for ( FriendNode*ptr=allUsers;ptr!=NULL;ptr=ptr->next)
	   {
		   if( in_friend_list(ptr->user->friends,user)==true)
		   {
			   ptr->user->friends=delete_from_friend_list(ptr->user->friends,user);
		   }
	   }
	   user->brands=delete_from_brand_list(user->brands,user->brands->brand_name);
	   allUsers=delete_from_friend_list(allUsers,user);
	   return 0;
	 }
   return -1;
}

/**
 * TODO: Complete this function
 * Create a friendship between user and friend.
 Given a pair of valid users, create a friendship.
 A user's friends list must remain in alphabetical order.
 Return 0 if the friendship was successfully created. Return -1 if the pair were already friends.
 */
int add_friend(User *user, User *friend)
{
	if(in_friend_list(user->friends,friend)!=true && in_friend_list(friend->friends,user)!=true && strcmp(user->name,friend->name)!=0)
	{
		
		user->friends=insert_into_friend_list(user->friends,friend);
		
		friend->friends=insert_into_friend_list(friend->friends,user);
		return 0;
	}
	 return -1;
}

/**
 * TODO: Complete this function
 * Removes a friendship between user and friend.Given a pair of valid users, remove their friendship. 
 A user's friends list must remain in alphabetical order. Return 0 if the pair are no longer friends. 
   Return -1 if the pair were not friends to begin with.
 */
 
 
int remove_friend(User *user, User *friend)
      
{
	if (in_friend_list(user->friends,friend)==true)
	{
		user->friends=delete_from_friend_list(user->friends,friend);
		return 0;
	}
	if (in_friend_list(friend->friends,user)==true)
	{
		friend->friends=delete_from_friend_list(friend->friends,user);
		return 0;
	}
  return -1;
}

/**
 * TODO: Complete this function
 * Creates a follow relationship, the user follows the brand.
 Given a valid user and the name of a brand, create a link between the user and the brand. 
 A user's brands list must remain in alphabetical order. Return 0 if the link was successfully created. 
 Return -1 if the link already existed or if the brand name is invalid
 
 */
int follow_brand(User *user, char *brand_name)
{
	
	if(in_brand_list(user->brands,brand_name)!=true && get_brand_index(brand_name)!=-1)
	{
	
	  user->brands=insert_into_brand_list(user->brands,brand_name);
	  return 0;
	}
	
  return -1;
}

/**
 * TODO: Complete this function
 * Removes a follow relationship, the user unfollows the brand.
 Given a valid user and the name of a brand, remove the link between the user and the brand. 
 A user's brands list must remain in alphabetical order. 
 Return 0 if the link was successfully removed. 
 Return -1 if the link did not previously exist or if the brand name is invalid.

 */
int unfollow_brand(User *user, char *brand_name)
{
	
	if(in_brand_list(user->brands,brand_name)==true && get_brand_index(brand_name)!=-1)
	{
		user->brands=delete_from_brand_list(user->brands,brand_name);
		return 0;
	}
		
	
  return -1;
}

/**
 * TODO: Complete this function
 * Return the number of mutual friends between two users.
 Given a pair of valid users, return the number of mutual friends between them. 
 A mutual friend is a user that exists in the friends list of both User a and User b.
 */
int get_mutual_friends(User *a, User *b) 
{
	int Mutual_friend=0;
	for (FriendNode *cur = a->friends; cur != NULL; cur = cur->next)
	{
		for (FriendNode *cur2 =b->friends;cur2 != NULL;cur2 = cur2->next)
		{
			if (strcmp(cur->user->name,cur2->user->name)==0)
			{
				Mutual_friend++;
			}
		}
	}
	
  return Mutual_friend;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//HELPER FUNCTIONS FOR GET DEGREES OF CONNECTION




Q_node*create_Q_nodes(User*user)
{
	Q_node*q_node=(Q_node*)calloc(1,sizeof(Q_node));
	q_node->user=user;
	q_node->next=NULL;
	return q_node;
}

Queue*create_q(Queue*q)
{
	q=(Queue*)calloc(1,sizeof(Queue));
	q->front=NULL;
	q->end=NULL;
	return q;
}





void set_all_visit_to_false(FriendNode*head)
{
	for ( FriendNode*cur=head;cur!=NULL;cur=cur->next)
	{
		cur->user->visited=false;
	}
}

void set_all_dist_to_neg1(FriendNode*head)
{
	for ( FriendNode*cur=head;cur!=NULL;cur=cur->next)
	{
		cur->user->dist=-1;
	}
}


int isempty(Queue*q)
{
	if ( q->front==NULL && q->end==NULL)
	{
		return 1;
	}
	return 0;
}


Queue*enqueue(Queue*q,User*user)
{
   Q_node*user_node=create_Q_nodes(user);
   if ( q->end==NULL)
   {
	   q->front=user_node;
	   q->end=user_node;
   }
   q->end->next=user_node;
   q->end=user_node;
   return q;
}


void dequeue(Queue*q)
{
	if ( q->front==NULL)
	{
		
       return;
	}
    else
	{
		 Q_node*temp=q->front->next;
		 q->front=q->front->next;
		 if(q->front==NULL)
		 {
			 q->end=NULL;
			 free(temp);
		 }
	 
	}

}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * TODO: Complete this function
 * A degree of connection is the number of steps it takes to get from
 * one user to another. Returns a non-negative integer representing
 * the degrees of connection between two users.
 
 Given a pair of valid users, return the degree of connection between both users. 
 The degree of connection is the shortest number of steps it takes to get from one user to the other. 
 If a connection cannot be formed, return -1.

 */
int get_degrees_of_connection(User *a, User *b)
{
	set_all_visit_to_false(allUsers);
	set_all_dist_to_neg1(allUsers);
	a->visited=true;
	a->dist=a->dist+1;
	Queue*bfs_q=NULL;
	bfs_q=create_q(bfs_q);
	bfs_q=enqueue(bfs_q,a);
	while(isempty(bfs_q)==0)
	{
		for ( FriendNode*cur=bfs_q->front->user->friends; cur!=NULL; cur=cur->next)
		{
		
			if ( cur->user->visited!=true)
			{
				cur->user->visited=true;
				cur->user->dist=bfs_q->front->user->dist+1;
				bfs_q=enqueue(bfs_q,cur->user);
				
			}
		}
		if ( strcmp(bfs_q->front->user->name,b->name)==0)
		{
			return bfs_q->front->user->dist;
		}
		else
		{
		       dequeue(bfs_q);
		}
			
	}
	free(bfs_q);
	return -1;
	
}


/**
 * TODO: Complete this function
 * Marks two brands as similar.
 Given two brand names, mark the two brands as similar in the brand_adjacency_matrix variable. 
 If either brand name is invalid, do nothing.
 
 */
void connect_similar_brands(char *brandNameA, char *brandNameB)
{
	if ( get_brand_index(brandNameA)!=-1 && get_brand_index(brandNameB)!=-1)
	{
		int ind1=get_brand_index(brandNameA);
		int ind2=get_brand_index(brandNameB);
		brand_adjacency_matrix[ind1][ind2]=1;
		brand_adjacency_matrix[ind2][ind1]=1;
	}
  return;
}

/**
 * TODO: Complete this function
 * Marks two brands as not similar.
 Given two brand names, mark the two brands as different (not similar) in the brand_adjacency_matrix variable. 
 If either brand name is invalid, do nothing.
 
 */
void remove_similar_brands(char *brandNameA, char *brandNameB)
{
	if ( get_brand_index(brandNameA)!=-1 && get_brand_index(brandNameB)!=-1)
	{
		int ind1=get_brand_index(brandNameA);
		int ind2=get_brand_index(brandNameB);
		brand_adjacency_matrix[ind1][ind2]=0;
		brand_adjacency_matrix[ind2][ind1]=0;
	}
  return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HELPER FUNCTIONS  FOR GET SUGGESTED FRIENDS 



int count_user_brands(User*usera , User*userb)
{ 
	   /** to count the number of mutual brands the users follow **/
	   
	   
		int count=0;
		for (BrandNode *cur = usera->brands; cur != NULL; cur = cur->next)
		{
			for(BrandNode *cur2 = userb->brands; cur2!= NULL;cur2 = cur2->next)
			{
				if (strcmp(cur->brand_name,cur2->brand_name)==0)
				{
					count++;
				}
			}
		}
		return count;
}


int find_the_length_of_user_friends(FriendNode*head)
{
	int count=0;
	for ( FriendNode*cur=head;cur!=NULL;cur=cur->next)
	{
          count++;
	}
	return count;
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * TODO: Complete this function
 * Returns a suggested friend for the given user.
 Given a user, suggest a new friend for them. To find the best match, 
 the new suggested friend should have the highest number of mutually liked brands amongst all other valid candidates. 
 If a tie needs to be broken, select the user with the name that comes first in reverse-alphanumerical order. 
 The suggested friend must be a valid user, cannot be the user themself, nor cannot be some that they're already friends with. 
 If the user is already friends with everyone on the platform, return NULL. 
 Also, at times, you might add users with no mutually liked brands; 
 worst comes to worse, at times, a similarity rating of 0 is the best option.
 */
User*get_suggested_friend(User *user)
{ 
    User*suggested=NULL;
	int max=-1;
	for ( FriendNode*cur=allUsers->next;cur!=NULL;cur=cur->next)
	{
		if ( in_friend_list(user->friends,cur->user)==false && strcmp(cur->user->name, user->name)!=0)
		{
			
			if ( count_user_brands(user,cur->user)>=max)
			{
				
				suggested=cur->user;
				max=count_user_brands(user,cur->user);
			}
			else if ( count_user_brands(user,cur->user)==max)
			{
				if ( strcmp(suggested->name,cur->user->name)>0)
				{
				       suggested=cur->user;
				}
			}
			
		}
	}
	return suggested;
  
  		
}

/**
 * TODO: Complete this function
 * Adds n suggested friends for the given user.
 * Returns how many friends were successfully followed.
 Given a user and a positive integer n,
 add n suggested friends using the get_suggested_friend() function. 
 There might not be enough users on the platform to sastify n complete,
 so return the amount of friends successfully added.
*/
int add_suggested_friends(User *user, int n)
{
	int num=0;
	int success;
	int len=find_the_length_of_user_friends(user->friends);
	int allUsers_len=find_the_length_of_user_friends(allUsers);
	int rem_num=(allUsers_len-1)-len;
	
	if(n>rem_num)
	{
		n=rem_num;
	}
	for ( int i=0;i<n;i++)
	{
				User*get_friend=get_suggested_friend(user);
				if ( get_friend!=NULL)
				{
					success=add_friend(user,get_friend);
					if (success==0)
					{  
						num++;
					}
				}
	}

	return num; 
	
		

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// HELPER FUNCTIONS FOR FOLLOW SUGGESTED BRANDS

BrandNode*find_sim_brands(User*user,int n)
{
	char brand_to_suggest[MAX_STR_LEN]="\0";
	BrandNode*brand_sim_h=NULL; // head for the new linked list of similar brands 
	int count; // to keep a count of similarity 
	for ( int num_times=0; num_times<n;num_times++)
	{    int max=-1;
			for ( int row=0; row<MAT_SIZE;row++) // go through rows 
			{
				if ( in_brand_list(user->brands,brand_names[row])==false && in_brand_list(brand_sim_h,brand_names[row])==false)
				{
				    count=0;
					for ( int col=0;col<MAT_SIZE;col++) // loop through the cols
					{
						if ( in_brand_list(user->brands,brand_names[col])==true)
						{
							  if (brand_adjacency_matrix[row][col]==1) // 1,0
								{ 
									   count++;
								}
							 
						}
						   
				    }
						
			       
					if (max<count) //if not tie
					{
						   max=count;
						   strcpy(brand_to_suggest,brand_names[row]);
						   
			
				    }
					else if ( max==count) // if tie
					{
						   if ( strcmp(brand_names[row],brand_to_suggest)>0)
						   {
								strcpy(brand_to_suggest,brand_names[row]);
								
						   }
					}
				
			    }
		    }
			if ( in_brand_list(brand_sim_h,brand_to_suggest)==false)
			{
			    brand_sim_h=insert_into_brand_list(brand_sim_h,brand_to_suggest);
			}
			
	    }
	
	     return brand_sim_h;	
			
	}	


  




int find_the_length_of_user_brands(User*user)
{
	int count=0;
	for ( BrandNode*cur=user->brands;cur!=NULL;cur=cur->next)
	{
          count++;
	}
	return count;
	
}

BrandNode*free_suggested_brands_linked_list(BrandNode*head)
{
	BrandNode*ptr=NULL;//p
    BrandNode*temp=NULL;//q
    ptr=head;
    while(ptr!=NULL)
    {
	   temp=ptr->next;
	   free(ptr);
	   ptr=temp;
    }
  return NULL;
}




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * TODO: Complete this function
 * Follows n suggested brands for the given user.
 * Returns how many brands were successfully followed.
 Given a user and a positive interger n, suggest n new brands for them. 
 To find the best matches, suggested brands with the most similarities with the brands that the user already follows.
 If a tie needs to be broken, 
 select the brand with the name that comes first in reverse-alphanumerical order. 
 The suggested brand must be a valid brand and cannot be a brand that the user already follows. 
 There might not be enough brands on the platform to sastify n complete, so return the amount of brands successfully followed. 
 Like add_suggested_friends, sometimes, 
 adding a brand with a similarity rating of 0 is the best option.
 */
int follow_suggested_brands(User *user, int n)
{
	int num=0;
	int success;
	int num_of_brands=find_the_length_of_user_brands(user);
	int rem_num=MAT_SIZE-num_of_brands;
	
	if(n>rem_num)
	{
		n=rem_num;
	}
	BrandNode*h=find_sim_brands(user,n);
	if ( h!=NULL)
	{
		for ( BrandNode*cur=h;cur!=NULL;cur=cur->next)
		{
			success=follow_brand(user,cur->brand_name);
			if ( success==0)
			{
				  num++;
			}
			
		}
	}
	h=free_suggested_brands_linked_list(h);
	return num;
		
					
}



////////////////////////////////////////////


	
	
	
	
	
	
	
	
	
	

