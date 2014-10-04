//#include <iostream>
#include "raskroy.h"

// TODO: ���������� ������� (���������� ������� ���������� ������� �����), ��������� �����.
// TODO: �������� �������� ������.
//		�������� �����!!!

namespace raskroy
{

	gilotine::t_other_size::t_other_size(void)
		: n(0)
	{
	}

	void gilotine::t_other_size::add_part(const t_part &part) {
		parts.push_back(part);
		n += part.ammount;
	};

	gilotine::t_other_sizes::iterator gilotine::t_other_sizes::find(scalar size)
	{
		for (iterator i = begin(); i != end(); i++)
			if (i->size == size)
				return i;
		return end();
	}

	void gilotine::t_other_sizes::add_other_size(unsigned s, const t_part &part)
	{
		t_other_size os;
		os.size = part.list.size[!s];
		os.add_part(part);
		push_back(os);
	}

	void gilotine::t_other_sizes::prepare(void)
	{
		min = begin();
		iterator i = begin();
		i++;
		for (; i != end(); i++)
			if (i->size < min->size)
				min = i;
	}

	gilotine::t_other_sizes::const_iterator gilotine::t_other_sizes::watchMin(void) const
	{
		assert(min);
		return min;
	}

	gilotine::t_sizes::iterator gilotine::t_sizes::find(scalar size)
	{
		for (iterator i = begin(); i != end(); i++)
			if (i->size == size)
				return i;
		return end();
	}

	void gilotine::t_sizes::add_size(unsigned s, const t_part &part)
	{
		t_size size;
		size.size = part.list.size[s];
		size.other_sizes.add_other_size(s, part);
		push_back(size);
	}

	gilotine::t_ammounts& gilotine::t_ammounts::operator += (const gilotine::t_ammounts &ammounts)
	{
		assert(size() == ammounts.size());
		iterator i1 = begin();
		const_iterator i2 = ammounts.begin();
		for (; i1 != end(); i1++, i2++)
			*i1 += *i2;
		return *this;
	}

	gilotine::t_ammounts& gilotine::t_ammounts::operator -= (const gilotine::t_ammounts &ammounts)
	{
		assert(size() == ammounts.size());
		iterator i1 = begin();
		const_iterator i2 = ammounts.begin();
		for (; i1 != end(); i1++, i2++)
			*i1 -= *i2;
		return *this;
	}

	gilotine::t_ammounts& gilotine::t_ammounts::operator *= (unsigned n)
	{
		for (iterator i = begin(); i != end(); i++)
			(*i) *= n;
		return *this;
	}

	gilotine::t_ammounts gilotine::t_ammounts::operator - (const gilotine::t_ammounts &a2) const
	{
		assert(size() == a2.size());
		t_ammounts res;
		res.resize(size());
		const_iterator i1 = begin();
		const_iterator i2 = a2.begin();
		iterator ri = res.begin();
		for (; i1 != end(); i1++, i2++, ri++)
		{
			(*ri) = (*i1) - (*i2);
		}
		return res;
	}

	gilotine::t_ammounts gilotine::t_ammounts::operator * (unsigned n) const
	{
		t_ammounts res;
		res.resize(size());
		const_iterator i = begin();
		iterator ri = res.begin();
		for (; i != end(); i++, ri++)
		{
			(*ri) = (*i) * n;
		}
		return res;
	}

	unsigned gilotine::t_ammounts::operator / (const gilotine::t_ammounts &a2) const
	{
		assert(size() == a2.size());
		unsigned mind;
		bool first = false;
		const_iterator i1 = begin();
		const_iterator i2 = a2.begin();
		for (; i1 != end(); i1++, i2++)
		{
			if (*i2 == 0)
				continue;

			unsigned d = (*i1)/(*i2);
			if (d == 0)
				return 0;
			if (first || d < mind)
			{
				first = false;
				mind = d;
			}
		}
		assert(!first);
		return mind;
	}

// ����������� �������
// ���������:
//		[i] size - ������ �� ������� ����� ����������� ������
//		[o] rashod - ���������� ������������� �������
scalar gilotine::recursive_perebor(scalar size, t_ammounts &rashod)
{
	//if (i == perebor_other_sizes->end())
	//	return size;
	scalar register size1;
	//t_ammounts best_ammounts(ammounts.size());
	unsigned n;
	if (perebor_i != perebor_end)
	{
		n = 0;
		scalar best = size;
		size1 = size;
		t_ammounts rashod1;
		rashod1.resize(rashod.size());
		std::fill(rashod1.begin(), rashod1.end(), 0);
		while (size1 > factory.saw_thickness && n < remains[perebor_i->offset])
		{
			scalar remain;
			//if (size1 < perebor_minimum_size - factory.saw_thickness)
			//	remain = size1;
			//else
			//{
				perebor_i++;
				remain = recursive_perebor(size1, rashod1);
				perebor_i--;
			//}
			if (remain < best)
			{
				best = remain;
				//perebor_i->n = n;
				std::copy(rashod1.begin(), rashod1.end(), rashod.begin());
				rashod[perebor_i->offset] = n;
			}
			n++;
			size1 = size - (perebor_i->size+factory.saw_thickness)*n;
		}
		//ammounts = best_ammounts;
		return best;
	}
	else
	{
		n = unsigned((size+factory.saw_thickness)/(perebor_i->size+factory.saw_thickness));
		if (n > remains[perebor_i->offset])
			n = remains[perebor_i->offset];
		rashod[perebor_i->offset] = n;
		size1 = size - n*(perebor_i->size+factory.saw_thickness);
		return size1;
	}
}

}

// ������� ������� ������ ��������� ������� �� ������ ������� � ��������� ��������
// ���������:
//		[i] size - �������� ������
//		[i] other_size - ���������������� ������
//		[o] stat - ����������
//		[o] raskroy - ������������ �������
//		[o] rashod - ������ �������
// ���������� true ���� ���� �� ���� ������ �����������
bool raskroy::gilotine::perebor(const t_size &size, scalar other_size, t_stat &stat, t_raskroy &raskroy, t_ammounts &rashod)
{
	if (other_size < size.other_sizes.watchMin()->size)
		return false;

	unsigned nums = 0;
	perebor_i = size.other_sizes.begin();
	perebor_end = size.other_sizes.end()-1;
	scalar remain = recursive_perebor(other_size, rashod);
	if (remain < other_size)
	{
		for (perebor_i = size.other_sizes.begin(); perebor_i != size.other_sizes.end(); perebor_i++)
			if (rashod[perebor_i->offset] > 0)
			{
				t_raskroy::t_detail detail;
				detail.size = perebor_i->size;
				detail.num = rashod[perebor_i->offset];
				raskroy.details.push_back(detail);
				nums += rashod[perebor_i->offset];
			}
		stat.sum_cut_length = size.size*(nums);
		stat.sum_remain = size.size*factory.saw_thickness;
		perebor_remain = remain;
		return true;
	}
	else
		return false;
}

// ����������� ������� ���� ������� ����� �� �����/������ (s=0/1) � ������������ �������� ������������
// ���������:
//		[i] list - ������� �����
//		[i/o] stat - ����������
//		[i] s - ����� ����������� ������� �� �����/������ s=0/1
//		[o] raskroy - ������� �����
//		[o] rashod - ������ �������
//
bool raskroy::gilotine::recursive(const t_list &list, t_stat &stat, unsigned s, t_raskroy &raskroy, t_ammounts &rashod)
{
	recursion_depth++;
	if (recursion_depth > max_recursion_depth)
		goto nothing;
	if (list.size[s] < minimum_size[s])
		goto nothing;
	{
	bool first = true;
	t_stat best_stat;
	t_ammounts best_rashod;
	t_ammounts rashod1;
	rashod1.resize(remains.size());
	for (t_sizes::iterator i = sizes[s].begin(); i != sizes[s].end(); i++)
	{
		t_stat stat1;
		if (i->size > list.size[s])
			continue;
		else
		{
			t_raskroy raskroy1;
			std::fill(rashod1.begin(), rashod1.end(), 0);
			raskroy1.cut = i->size;
			raskroy1.s = s;
			stat1.sum_cut_length += list.size[!s];
			stat1.sum_remain += list.size[!s]*factory.saw_thickness;

			if (perebor(*i, list.size[!s], stat1, raskroy1, rashod1))
			{
				t_list list1;
				list1.size[s] = i->size;
				list1.size[!s] = perebor_remain;
				t_raskroy remain_raskroy;
				t_ammounts remain_rashod;
				remains -= rashod1;
				if (recursive(list1, stat1, !s, remain_raskroy, remain_rashod))
				{
					remains += rashod1;
					raskroy1.attachRemain(remain_raskroy);
					rashod1 += remain_rashod;
				}
				else
					remains += rashod1;
			}
			else
				continue;

			unsigned size_krat = (list.size[s] + factory.saw_thickness)/(i->size + factory.saw_thickness);
			unsigned kol_krat = remains/rashod1;
			if (size_krat > kol_krat)
				raskroy1.kratnostj = kol_krat;
			else
				raskroy1.kratnostj = size_krat;

			if (raskroy1.kratnostj == 0)						   
				assert(raskroy1.kratnostj);

			if (raskroy1.kratnostj > 1)
			{
				rashod1 *= raskroy1.kratnostj;
				stat1 *= raskroy1.kratnostj;
			}
			stat1 += stat;

			if (!first && pcriteria->compare(best_stat, stat1))	// already bad
				continue;
			
			if ((i->size + factory.saw_thickness)*raskroy1.kratnostj > list.size[s] - minimum_size[s])
			{
				stat1.sum_remain += (list.size[s] - (i->size + factory.saw_thickness)*raskroy1.kratnostj)*list.size[!s];
			}
			else
			{
				t_list list2(list);
				list2.size[s] = list.size[s] - (i->size + factory.saw_thickness)*raskroy1.kratnostj;
				t_raskroy recurse_raskroy;
				if (bylen_bywid(list2, stat1, recurse_raskroy, rashod1))
					raskroy1.attachRecurse(recurse_raskroy);
			}

			if (first || pcriteria->compare(stat1, best_stat))
			{
				best_stat = stat1;
				raskroy = raskroy1;
				best_rashod = rashod1;
				first = false;
			}
		}
	}
	if (!first)
	{
		stat = best_stat;
		rashod = best_rashod;
		recursion_depth--;
		return true;
	}
	else
	{
		goto nothing;
	}
	}
nothing:
	stat.sum_remain += list.size[0]*list.size[1];
	stat.remains_num++;
	recursion_depth--;
	return false;
}

// ������� ����� �� ����� � �� ������, ���������� ������ �������
// ���������:
//		[i] list - ������ �����
//		[i/o] stat - ����������
//		[o] raskroy - ������� �����
//		[i/o] rashod - ������ �������
//
bool raskroy::gilotine::bylen_bywid(const t_list &list, t_stat &stat, t_raskroy &raskroy, t_ammounts &rashod)
{
	if (recursion_depth+1 > max_recursion_depth)
		goto nothing;

	{
	t_stat stat1(stat), stat2(stat);
	t_ammounts rashod1, rashod2;
	t_raskroy raskroy1, raskroy2;

	remains -= rashod;
	bool rb1 = recursive(list, stat1, 0, raskroy1, rashod1);
	bool rb2 = false;
	if (rb1)
		rb2 = recursive(list, stat2, 1, raskroy2, rashod2);
	remains += rashod;
	if (rb1 && rb2)
	{
		if (pcriteria->compare(stat1, stat2))
		{
			stat = stat1;
			raskroy = raskroy1;
			rashod += rashod1;
		}
		else
		{
			stat = stat2;
			raskroy = raskroy2;
			rashod += rashod2;
		}
		return true;
	}
	/*else if (rb1)
	{
		MessageBox(NULL, "hhh", 0, 0);
		stat = stat1;
		raskroy = raskroy1;
		return true;
	}
	else if (rb2)
	{
		MessageBox(NULL, "hhh", 0, 0);
		stat = stat2;
		raskroy = raskroy2;
		return true;
	}*/
	else
		goto nothing;
	}

nothing:
	stat.sum_remain += list.size[0]*list.size[1];
	stat.remains_num++;
	return false;
}

void raskroy::gilotine::make_materials_list(t_parts parts, t_parts lists)
{
	while (!parts.empty())
	{
		t_parts::iterator i = parts.begin();
		t_material mtrl;
		mtrl.id = i->material_id;
		//t_parts::iterator j = parts.begin();
		while (i != parts.end())
		{
			if (i->material_id == mtrl.id)
			{				
				mtrl.parts.push_back(*i);
				parts.erase(i);
				i = parts.begin();				
			}
			else
				i++;
		}
	
		t_parts::iterator j = lists.begin();
		while (j != lists.end())
		{
			if (j->material_id == mtrl.id)
			{
				mtrl.lists.push_back(*j);
				lists.erase(j);
				j = lists.begin();
			}
			else
				j++;
		}
		materials.push_back(mtrl);
	}
}

void raskroy::gilotine::add_part_to_sizes_list(unsigned s, const t_part &part)
{
	t_sizes::iterator sizei = sizes[s].find(part.list.size[s]);
	if (sizei != sizes[s].end())
	{
		t_other_sizes::iterator other_sizei = sizei->other_sizes.find(part.list.size[!s]);
		if (other_sizei != sizei->other_sizes.end())
			other_sizei->add_part(part);
		else
			sizei->other_sizes.add_other_size(s, part);
	}
	else
	{
		sizes[s].add_size(s, part);
		if (first || minimum_size[s] > part.list.size[s])
		{
			first = false;
			minimum_size[s] = part.list.size[s];
		}
	}
}

void raskroy::gilotine::make_sizes_lists_for_material(const t_material &material)
{
	sizes[0].clear();
	sizes[1].clear();

	for (unsigned s = 0; s <= 1; s++)
	{
		first = true;
		for (t_parts::const_iterator parti = material.parts.begin(); parti != material.parts.end(); parti++)
		{
			add_part_to_sizes_list(s, *parti);
			if (parti->rotate/* || material.rotate*/)
			{
				t_part rotpart(*parti);
				rotpart.list.size[0] = parti->list.size[1];
				rotpart.list.size[1] = parti->list.size[0];
				add_part_to_sizes_list(s, rotpart);
			}
		}
		/*if (material.rotate)
			s++;	// skip */
	}

	for (t_sizes::iterator sizei0 = sizes[0].begin(); sizei0 != sizes[0].end(); sizei0++)
	{
		for (t_other_sizes::iterator osi = sizei0->other_sizes.begin(); osi != sizei0->other_sizes.end(); osi++)
		{
			remains.push_back(osi->n);
			osi->offset = remains.size()-1;
		}
		sizei0->other_sizes.prepare();
	}
	for (t_sizes::iterator sizei1 = sizes[1].begin(); sizei1 != sizes[1].end(); sizei1++)
	{
		for (t_other_sizes::iterator osi = sizei1->other_sizes.begin(); osi != sizei1->other_sizes.end(); osi++)
			osi->offset = sizes[0].find(osi->size)->other_sizes.find(sizei1->size)->offset;
		sizei1->other_sizes.prepare();
	}
}

void raskroy::gilotine::remove_exosted_sizes(void)
{
	for (unsigned s = 0; s <= 1; s++)
	{
		t_sizes::iterator sizei = sizes[s].begin();
		while (sizei != sizes[s].end())
		{
			t_other_sizes::iterator osi = sizei->other_sizes.begin();
			while (osi != sizei->other_sizes.end())
			{
				if (remains[osi->offset] == 0)
				{
					sizei->other_sizes.erase(osi);
					osi = sizei->other_sizes.begin();
				}
				else
					osi++;
			}
			if (sizei->other_sizes.empty())
			{
				sizes[s].erase(sizei);
				sizei = sizes[s].begin();
			}
			else
				sizei++;
		}
	}
}


raskroy::gilotine::gilotine(void)
	: recursion_depth(0), max_recursion_depth(8)
{
}

// Raskroy start
void raskroy::gilotine::run(const t_parts &parts, const t_parts &lists, const t_factory &factory, const criteria &criteria)
{
	this->factory = factory;
	this->pcriteria = &criteria;
	make_materials_list(parts, lists);
	results.clear();
	results.cut_thickness = factory.saw_thickness;
	statistic.clear();
	for (t_materials::const_iterator mi = materials.begin(); mi != materials.end(); mi++)
	{
		make_sizes_lists_for_material(*mi);
		t_raskroy raskroy;
		t_result best_result;
		//best_result.torchevka = factory.
		t_ammounts rashod;
		rashod.resize(remains.size());
		while (1)
		{
			for (t_ammounts::const_iterator remainsi = remains.begin(); remainsi != remains.end(); remainsi++)
				if (*remainsi > 0)
					goto contine_raskroy;

			return;

contine_raskroy:
			bool first = true;
			for (t_parts::const_iterator li = mi->lists.begin(); li != mi->lists.end(); li++)
			{
				t_stat stat;
				stat.clear();
				std::fill(rashod.begin(), rashod.end(), 0);
				if (bylen_bywid(li->list, stat, raskroy, rashod))
					if (first || pcriteria->compare(stat, best_result.stat))
					{
						first = false;
						best_result.stat = stat;
						best_result.raskroy = raskroy;
						best_result.list = *li;
						best_result.ammount = remains/rashod;
						//if (best_result.ammount == 0)
						//	best_result.ammount = 1;
						if (best_result.ammount == 0)
							assert(best_result.ammount);
					}
			}
			if (first)
				throw std::exception("������ ����������� ������.");
			else
				results.push_back(best_result);
			remains -= rashod*best_result.ammount;
			remove_exosted_sizes();
			statistic += best_result.stat;
		}
	}
}