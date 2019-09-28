#include<iostream>
#include<string>
/*----------------------------------------------------FIFO{Second chance}------------------------------------------------------------*/
/*----------------------------------------------------[Made By: @Maromaro]-----------------------------------------------------------*/

/*Memory cases*/
typedef struct
{
	unsigned seniority;
	int val;
	bool ref,muted;
}Case;
class FIFO
{
private:
	Case*memory;
	unsigned int size,min,full;
public:
	FIFO(unsigned int _size)
	:size(_size)
	{
		memory = new Case[size];
		for (int i = 0; i < size; i++)
		{
			memory[i].val = -1;
		}
		full = 0;
	}
	~FIFO()
	{
		delete[] memory;
	}
	int find_Min()
	{
		int min;
		min = memory[0].seniority;
		for (int i = 0; i < size; i++)
		{
			if (memory[i].seniority < min)
				min = memory[i].seniority;
		}
		return min;
	}
	void Page_Replacement(int New_p)
	{
		if(full != size)
		{ 
			for (int i = 0; i < size; i++)
			{
				if (memory[i].val == -1 )
				{
					memory[i].val = New_p;
					memory[i].seniority = (i + 1);
					full++;
					break;
				}
			}
		}
		else if (full == size)
		{
			min = find_Min();
			for (int i = 0; i < size; i++)
			{
				if (memory[i].seniority == min)
				{
					memory[i].val = New_p;
					memory[i].seniority = size;
				}
				else
				{
					memory[i].seniority--;
				}
			}
		}
	
	}
	friend std::ostream& operator<<(std::ostream& out, FIFO& memory);
};
class FIFO_sc
{
private:
	Case*memory;
	unsigned int size;
	int full,min,tmp_Min;
	bool exist;
public:
	FIFO_sc(const unsigned int _size)
		:size(_size),exist(false)
	{
		memory = new Case[size];
		for (int i = 0; i < size; i++)
		{
			memory[i].val = -1;
			memory[i].muted = false;
		}
		full = 0;
	}
	~FIFO_sc()
	{
		delete[] memory;
	}
	int find_Min()const
	{
		int min = 10;
		for (int i = 0; i < size; i++) //Looking for the Min
		{
			if (memory[i].muted == false)
			{
				if (memory[i].seniority < min)
					min = memory[i].seniority;
			}
		}
		return min;
	}
	void page_R(int newV,int i)
	{
		memory[i].val = newV;
		memory[i].seniority = size;
		memory[i].ref = false;
	}
	int index_upd(int i)
	{
		int _i;
		if (i == (size - 1))
		{
			_i = 0;
		}
		else
		{
			_i = i + 1;
		}
		return _i;
	}
	void Page_Replacement(int newV)
	{
		exist = false;
		for (int i = 0; i < size; i++)
		{
			if (memory[i].val == newV)
			{
				exist = true;
				break;
			}
		}
		if (full != size && exist == false)
		{
			for (int i = 0; i < size; i++)
			{
				if (memory[i].val == -1)
				{
					memory[i].val = newV;
					memory[i].ref = false;
					memory[i].seniority = (i + 1);
					full++;
					break;
				}
			}
		}
		else if (full != size && exist == true)
		{
			for (int i = 0; i < size; i++)
			{
				if (memory[i].val == newV)
				{
					memory[i].ref = true;
					break;
				}
					
			}
		}
		else if(full == size)
		{
			min = find_Min();//Min of .seniority
			if (exist == false)
			{
				int _i,ref_cont =0;
				//if all .ref == 1
				for (int i = 0; i < size; i++)
				{
					if (memory[i].ref == true)
						ref_cont++;
				}
				if (ref_cont == size)
				{
					for (int i = 0; i < size; i++)
					{
						memory[i].ref = 0;
					}
				}
				ref_cont = 0;
				//Endif all .ref == 1
				for (int i = 0; i < size; i++)
				{
					if (memory[i].seniority == min && memory[i].ref == false && memory[i].val != newV)//Looking for the specifique Case
					{
						page_R(newV, i);//Page replacement
						memory[i].muted = true;
					}
					if (memory[i].seniority == min && memory[i].ref == true && memory[i].val != newV)//PROBLEM!!!!
					{
						memory[i].ref = false;
						memory[i].muted = true;
						tmp_Min = find_Min();//Reset the Min 'Temporary' 
						for (int j = 0; j < size; j++)
						{
							if(j == 0)
							_i = index_upd(i);

							if (memory[_i].seniority == tmp_Min && memory[_i].ref == false && memory[_i].val != newV)
							{
								page_R(newV, _i);//Page replacement
								memory[_i].muted = true;
								break;
							}
							else if (memory[_i].seniority == tmp_Min && memory[_i].ref == true && memory[_i].val != newV)
							{
								memory[_i].ref = false;
								memory[_i].muted = true;
								tmp_Min = find_Min();//Reset the Min 'Temporary' 
							}
							_i = index_upd(_i);
						}
					}
					else if (memory[i].seniority != min )//Update the .seniority of the rest
					{
						if (memory[i].muted == false)
							memory[i].seniority--;
					}
				}
				for (int i = 0; i < size; i++)
				{
					memory[i].muted = false;
				}
			
			}
			else
			{
				for (int i = 0; i < size; i++)
				{
					if (memory[i].ref == false && memory[i].val == newV)//Looking for the specifique Case
					{
						memory[i].ref = true;
						break;
					}
				}
			}
		}
	}
	friend std::ostream& operator<<(std::ostream& out, FIFO_sc& memory);
};
std::ostream& operator<<(std::ostream& out, FIFO_sc& memory)
{
	for (int i = 0; i < memory.size; i++)
	{
		out<< "Case Value: " << memory.memory[i].val << " Ref Bit: " << memory.memory[i].ref << "\nseniority: " << memory.memory[i].seniority << std::endl;
	}
	out << "------------------------------------------------------------------------\n" << std::endl;
	return out;
}
std::ostream& operator<<(std::ostream& out, FIFO& memory)
{
	for (int i = 0; i < memory.size; i++)
	{
		out << "Case Value: " << memory.memory[i].val <<"\nseniority: " << memory.memory[i].seniority << std::endl;
	}
	out << "------------------------------------------------------------------------\n" << std::endl;
	return out;
}
int main()
{
	int size,page,page_R;
	std::cout << "How many Cases you using :" << std::endl;
	std::cin >> size;
	std::cout << "How many page replacements  you willing to do :" << std::endl;
	std::cin >> page_R;
	system("cls");
	FIFO_sc test(size);
	for (int i = 0; i < page_R; i++)
	{
		std::cout << "New Page : " << std::endl;
		std::cin >> page;
		test.Page_Replacement(page);
		std::cout << test << std::endl;
	}
	return 0;
}