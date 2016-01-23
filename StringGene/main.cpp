#include <iostream>
#include <random>
#include <functional>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>

//Macro Function
#define GENE_RAND_SEED time(NULL)
#define GENE_STRN_OBJS std::string("Fucking Benedu and DimiLife")

//Const Variables
const size_t GENE_SIZE_POOL = 4096;
const size_t GENE_ALGO_LOOP = 16384;
const size_t GENE_RATE_ELIT = 0.10f;
const size_t GENE_RATE_MUTA = 0.25f;

struct SGene {
	std::string sGene;
	unsigned int uiFit;
};

typedef std::vector<SGene> vGene;

std::mt19937 engine(GENE_RAND_SEED);
std::uniform_int_distribution<unsigned int> distribution(0, RAND_MAX);
auto randGen = bind(distribution, engine);

inline bool sfuncCmp(SGene _a, SGene _b) {
	return (_a.uiFit < _b.uiFit);
}
inline void sortGene(vGene &_vgPool) {
	std::sort(_vgPool.begin(), _vgPool.end(), sfuncCmp);
}

void initGenePool(vGene &_vgPool, vGene &_vgNextGen) {
	size_t sztObj = GENE_STRN_OBJS.size();

	for (unsigned int i = 0; i < GENE_SIZE_POOL; i++) {
		SGene sgTmp;

		sgTmp.uiFit = 0;
		sgTmp.sGene.empty();

		for (unsigned int j = 0; j < sztObj; j++)
			sgTmp.sGene += (randGen() % 90) + 32;

		_vgPool.push_back(sgTmp);
	}

	_vgNextGen.resize(GENE_SIZE_POOL);
}

void fitGene(vGene &_vgPool) {
	size_t sztObj = GENE_STRN_OBJS.size();
	unsigned int uiTmpFit;

	for (unsigned int i = 0; i < GENE_SIZE_POOL; i++) {
		uiTmpFit = 0;
		for (unsigned int j = 0; j < sztObj; j++)
			uiTmpFit += abs(int(_vgPool[i].sGene[j] - GENE_STRN_OBJS[j]));
		_vgPool[i].uiFit = uiTmpFit;
	}
}

void mutGene(SGene& _sgGene) {
	size_t sztObj = GENE_STRN_OBJS.size();
	size_t sztPos = randGen() % sztObj;
	unsigned int uiDelt = (randGen() % 90) + 32;

	_sgGene.sGene[sztPos] = ((_sgGene.sGene[sztPos] + uiDelt) % 122);
}

void mateGene(vGene &_vgPool, vGene &_vgNextGen) {
	unsigned int iElit = GENE_SIZE_POOL * GENE_RATE_ELIT;
	size_t sztObj = GENE_STRN_OBJS.size(), sztPos;
	unsigned int uiVGPosA, uiVGPosB;

	for (unsigned int i = 0; i < iElit; i++) {
		_vgNextGen[i].sGene = _vgPool[i].sGene;
		_vgNextGen[i].uiFit = _vgPool[i].uiFit;
	}

	for (int i = iElit; i < GENE_SIZE_POOL; i++) {
		uiVGPosA = randGen() % (GENE_SIZE_POOL / 2);
		uiVGPosB = randGen() % (GENE_SIZE_POOL / 2);
		sztPos = randGen() % sztObj;

		_vgNextGen[i].sGene = _vgPool[uiVGPosA].sGene.substr(0, sztPos) +
			_vgPool[uiVGPosB].sGene.substr(sztPos, iElit - sztPos);

		if (randGen() < (RAND_MAX*GENE_RATE_MUTA)) mutGene(_vgNextGen[i]);
	}
}

inline void nextGenPool(vGene *&_vgPool, vGene *&_vgNextGen) {
	vGene *vgTmp;
	vgTmp = _vgPool;
	_vgPool = _vgNextGen;
	_vgNextGen = vgTmp;
}

void prtnGene(vGene &_vgPool, unsigned int _uiGeneration) {
	unsigned int uiTmpFit = 0;

	std::cout << std::endl << "************************************************************" << std::endl << "Generation : " << _uiGeneration << std::endl << std::endl;
	for (unsigned int i = 0; i < 10; i++) {
		std::cout << "Major Gene No." << i << " : " << _vgPool[i].sGene << std::endl;
		uiTmpFit += _vgPool[i].uiFit;
	}
	std::cout << std::endl << "Average Fitness : " << (uiTmpFit / 10) << std::endl << "Best Gene : " << _vgPool[0].sGene << std::endl << "************************************************************" << std::endl << std::endl;
}

int main(int argc, char** argv) {
	unsigned int uiGeneration = 1;
	vGene vgAllocA, vgAllocB;
	vGene *vgPool, *vgNextGen;

	initGenePool(vgAllocA, vgAllocB);
	vgPool = &vgAllocA;
	vgNextGen = &vgAllocB;

	for (unsigned int i = 0; i < GENE_ALGO_LOOP; i++) {
		fitGene(*vgPool);
		sortGene(*vgPool);
		prtnGene(*vgPool, uiGeneration);

		if ((*vgPool)[0].uiFit == 0) break;

		mateGene(*vgPool, *vgNextGen);
		nextGenPool(vgPool, vgNextGen);
		uiGeneration++;
	}

	return 0;
}