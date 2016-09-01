#include<bits/stdc++.h>
using namespace std;

typedef unsigned long int itemRange;
typedef unsigned long int maxNodes;

class trie {
    public:
        vector< vector<itemRange> > labels;
        vector< vector<maxNodes> > endNodes;
        vector<maxNodes> counts;
        vector<maxNodes> parent;
        vector< vector<maxNodes> > pairCounts;
        vector<itemRange> maxPathLen;
        vector<itemRange> sortedItemsByCount;
        vector<itemRange> posInSortedItems;
        maxNodes numRules;

        trie() {
            counts.push_back(0);
            maxPathLen.push_back(0);
            labels.resize(1);
            endNodes.resize(1);
            parent.push_back(0);
            numRules=0;
        }

        void genCandidatesUtil(maxNodes curNode,itemRange level,itemRange curLevel,
                set<itemRange>& buildCandidate) {
            itemRange curEdge;
            if(curLevel==level) {
                maxNodes nextNode;
                for(itemRange i=0;i<labels[curNode].size();i++) {
                    buildCandidate.insert(labels[curNode][i]);
                    nextNode=endNodes[curNode][i];
                    for(itemRange j=i+1;j<labels[curNode].size();j++) {
                        buildCandidate.insert(labels[curNode][j]);
                        if(checkImmediateSubsests(buildCandidate)) {
                            newNode(nextNode,labels[curNode][j],0);
                        }
                        buildCandidate.erase(labels[curNode][j]);
                    }
                    if(!labels[nextNode].empty()) maxPathLen[nextNode]=1;

                    buildCandidate.erase(labels[curNode][i]);
                }
                updateMaxPathLen(curNode);
            }
            else {
                for(curEdge=0;curEdge<labels[curNode].size();curEdge++) {
                    buildCandidate.insert(labels[curNode][curEdge]);
                    genCandidatesUtil(endNodes[curNode][curEdge],level,curLevel+1,buildCandidate);
                    buildCandidate.erase(labels[curNode][curEdge]);
                }
            }
        }

        void genCandidates(itemRange& level) {
            if(level==1) {
                if(!labels[0].empty()) {
                maxPathLen[0]=2;
                int pSize=labels[0].size()-1;
                pairCounts.reserve(pSize);
                pairCounts.resize(pSize);
                for(itemRange i=0; i<pSize; i++) {
                    pairCounts[i].reserve(pSize-i);
                    pairCounts[i].resize(pSize-i,0);
                }
            }
            }
            else if(level==maxPathLen[0]) {
                set<itemRange> buildCandidate;
                genCandidatesUtil(0,level,1,buildCandidate);
            }
        }

        void updateMaxPathLen(maxNodes node) {
            maxNodes maxLen=0;
            for(vector<itemRange>::iterator i=endNodes[node].begin();i!=endNodes[node].end();i++) {
                if(maxLen<maxPathLen[*i]+1) maxLen=maxPathLen[*i]+1;
            }
            if(maxPathLen[node]!=maxLen) {
                maxPathLen[node]=maxLen;
                if(node) updateMaxPathLen(parent[node]);
            }
        }

        void deleteEdge(maxNodes fromNode,maxNodes toNode) {
            vector<itemRange>::iterator i=lower_bound(endNodes[fromNode].begin(),endNodes[fromNode].end(),toNode);
            labels[fromNode].erase(i-endNodes[fromNode].begin()+labels[fromNode].begin());
            endNodes[fromNode].erase(i);
            if(labels[fromNode].empty()) {
                maxPathLen[fromNode]=0;
                updateMaxPathLen(parent[fromNode]);
            }
        }

        void newNode(maxNodes fromNode,itemRange item,unsigned long initCount) {
            labels[fromNode].push_back(item);
            endNodes[fromNode].push_back(labels.size());
            labels.resize(labels.size()+1);
            endNodes.resize(labels.size());
            parent.push_back(fromNode);
            counts.push_back(initCount);
            maxPathLen.push_back(0);
        }

        maxNodes isPresent(set<itemRange>& itemSet) {
            maxNodes curNode=0;
            vector<itemRange>::iterator itNode;
            for(set<itemRange>::iterator it=itemSet.begin();it!=itemSet.end();it++) {
                itNode=lower_bound(labels[curNode].begin(),labels[curNode].end(),*it);
                if(itNode==labels[curNode].end() or *it!=*itNode) return 0;
                curNode=endNodes[curNode][itNode-labels[curNode].begin()];
            }
            return curNode;
        }

        bool checkImmediateSubsests(set<itemRange>& buildCandidate) {
            if(buildCandidate.size()<=2) return true;
            set<itemRange> tempCandidate(buildCandidate);
            for(set<itemRange>::iterator it=buildCandidate.begin();it!=buildCandidate.end();it++) {
                tempCandidate.erase(*it);
                if(!isPresent(tempCandidate)) return false;
                tempCandidate.insert(*it);
            }
            return true;
        }

        void findCandidateOne(const vector<itemRange>& record) {
            counts[0]++;
            for(vector<itemRange>::const_iterator it=record.begin();it!=record.end();it++) {
                if(*it+1>=counts.size()) counts.resize(*it+2,0);
                counts[*it+1]++;
            }
        }

        void findCandidateTwo(const vector<itemRange>& record, unsigned long int tCnt) {
            vector<itemRange>::const_iterator it1,it2;
            for(it1=record.begin();it1!=record.end()-1;it1++) {
                for(it2=it1+1;it2!=record.end();it2++) {
                    pairCounts[*it1-1][*it2-*it1-1]+=tCnt;
                }
            }
        }

        void findCandidateMore(const vector<itemRange>& record,itemRange candidateSize,
                vector<itemRange>::const_iterator start,maxNodes curNode,
                maxNodes curSize,unsigned long int tCnt) {
            if(candidateSize==curSize) counts[curNode]+=tCnt;
            else {
                vector<maxNodes>::iterator itNode=endNodes[curNode].begin();
                for(vector<itemRange>::iterator it=labels[curNode].begin();it!=labels[curNode].end() and candidateSize<=curSize+record.end()-start;) {
                    if(*it<*start) {
                        it++;
                        itNode++;
                    }
                    else if(*it>*start) {
                        start++;
                    }
                    else {
                        if(candidateSize==maxPathLen[*itNode]+curSize+1) {
                            findCandidateMore(record,candidateSize,start+1,*itNode,curSize+1,tCnt);
                        }
                        it++;
                        itNode++;
                        start++;
                    }
                }
            }
        }

        void findCandidate(const vector<itemRange>& record,itemRange candidateSize,unsigned long int tCnt) {
            if(candidateSize==1) findCandidateOne(record);
            else if(candidateSize==2) findCandidateTwo(record,tCnt);
            else findCandidateMore(record,candidateSize,record.begin(),0,0,tCnt);
        }

        void deleteInfrequentOne(unsigned long int minCount) {
            itemRange curEdge;
            posInSortedItems.reserve(counts.size());
            posInSortedItems.resize(counts.size(),0);
            sortedItemsByCount.resize(1);
            vector<maxNodes> tempCounts(1);
            vector<itemRange>::iterator it;

            for(curEdge=1;curEdge<counts.size();curEdge++) {
                if(counts[curEdge]>=minCount) {
                    it=lower_bound(tempCounts.begin()+1,tempCounts.end(),counts[curEdge]);
                    sortedItemsByCount.insert(sortedItemsByCount.begin()+(it-tempCounts.begin()),curEdge);
                    tempCounts.insert(it,counts[curEdge]);
                }
            }

            if(sortedItemsByCount.size()>1) maxPathLen[0] = 1;
            vector<itemRange>(sortedItemsByCount).swap(sortedItemsByCount);
            counts.resize(1);
            for(curEdge=1;curEdge<sortedItemsByCount.size();curEdge++)
                newNode(0,curEdge,tempCounts[curEdge]);
            for(curEdge=1;curEdge<sortedItemsByCount.size();curEdge++)
                posInSortedItems[sortedItemsByCount[curEdge]]=curEdge;
        }

        void deleteInfrequentTwo(unsigned long int minCount) {
            maxNodes node1,node2;
            for(node1=1;node1<labels[0].size();node1++) {
                for(node2=0;node2<labels[0].size()-node1;node2++) {
                    if(pairCounts[node1-1][node2]>=minCount) {
                        newNode(node1,node1+node2+1,pairCounts[node1-1][node2]);
                    }
                }
                if(!labels[node1].empty()) maxPathLen[node1]=1;
                pairCounts[node1-1].clear();
                vector<maxNodes>().swap(pairCounts[node1-1]);
            }
            pairCounts.clear();
            vector< vector<maxNodes> >().swap(pairCounts);
            if(labels.size()==labels[0].size()+1) maxPathLen[0]=1;
        }

        void deleteInfrequentMore(unsigned long int minCount) {
            maxNodes node1=1,node2,par;
            for(node2=1;node2<labels.size();node2++) {
                par=parent[node2];
                if(counts[node2]>=minCount) {
                    if(node1!=node2) {
                        vector<maxNodes>::iterator it=lower_bound(endNodes[par].begin(),endNodes[par].end(),node2);
                        *it=node1;
                        labels[node1]=labels[node2];
                        endNodes[node1]=endNodes[node2];
                        parent[node1]=parent[node2];
                        counts[node1]=counts[node2];
                        maxPathLen[node1]=maxPathLen[node2];
                    }
                    node1++;
                }
                else deleteEdge(par,node2);
            }
            labels.resize(node1);
            endNodes.resize(node1);
            parent.resize(node1);
            counts.resize(node1);
            maxPathLen.resize(node1);
        }

        void deleteInfrequent(unsigned long int minCount) {
            if(maxPathLen[0]==0) deleteInfrequentOne(minCount);
            else if(maxPathLen[0]==2) deleteInfrequentTwo(minCount);
            else deleteInfrequentMore(minCount);
        }

        void genOutputUtil(ofstream& outFile,maxNodes curNode,itemRange freqSize,itemRange curSize,set<itemRange>& freqSet) {
            if(curSize==freqSize) {
            	set<itemRange>::iterator it;
                for(it=freqSet.begin();it!=--(freqSet.end());it++) {
                    outFile<<sortedItemsByCount[*it]-1<<',';
                }
                outFile<<sortedItemsByCount[*it]-1<<endl;
            }
            else {
                vector<itemRange>::iterator itNode=endNodes[curNode].begin();
                vector<itemRange>::iterator it;
                for(it=labels[curNode].begin();it!=labels[curNode].end();it++,itNode++) {
                    if(maxPathLen[*itNode]+curSize+1>=freqSize) {
                        freqSet.insert(*it);
                        genOutputUtil(outFile,*itNode,freqSize,curSize+1,freqSet);
                        freqSet.erase(*it);
                    }
                }
            }
        }

        void genOutput(ofstream& outFile) {
        	outFile<<"#,"<<getTotalNodes()-1<<endl;
            for(itemRange freqSize=1;freqSize<maxPathLen[0]+1;freqSize++) {
                set<itemRange> freqSet;
                genOutputUtil( outFile,0,freqSize,0,freqSet );
            }
        }

        void mineRuleFind(ofstream& outFile,double minconf,set<itemRange>& lhs,set<itemRange>& rhs,unsigned long unionCount) {
            set<itemRange>::iterator itl,itr;
            itemRange item;
            for(itr=rhs.begin();itr!=rhs.end();itr++) {
                if(lhs.empty() or *(--lhs.end())<*itr) {
                    item=*itr;
                    rhs.erase(item);
                    lhs.insert(item);
                    if(unionCount>counts[isPresent(lhs)]*minconf) {
                        numRules++;
                        for(itl=lhs.begin();itl!=--(lhs.end());itl++)
                            outFile<<sortedItemsByCount[*itl]-1<<',';
                        outFile<<sortedItemsByCount[*itl]-1;
                        outFile<<",=>,";
                        for(itl=rhs.begin();itl!=--(rhs.end());itl++)
                            outFile<<sortedItemsByCount[*itl]-1<<',';
                        outFile<<sortedItemsByCount[*itl]-1<<endl;
                    }
                    else if(rhs.size()>1) mineRuleFind(outFile,minconf,lhs,rhs,unionCount);
                    itr=(rhs.insert(item)).first;
                    lhs.erase(item);
                }
            }
        }

        void mineRulesUtil(ofstream& outFile,double minconf,maxNodes curNode,set<itemRange>& rhs) {
            if(rhs.size()>1) {
                set<itemRange> lhs;
                mineRuleFind(outFile,minconf,lhs,rhs,counts[curNode]);
            }
            vector<maxNodes>::iterator itNode=endNodes[curNode].begin();
            vector<itemRange>::iterator it;
            for(it=labels[curNode].begin();it!=labels[curNode].end();it++,itNode++) {
                rhs.insert(*it);
                mineRulesUtil(outFile,minconf,*itNode,rhs);
                rhs.erase(*it);
            }
        }

        void mineRules(ofstream& outFile,double minconf) {
            set<itemRange> rhs;
            ofstream tempRuleOutputPtr("./tempRuleOutput.csv");
            mineRulesUtil(tempRuleOutputPtr,minconf,0,rhs);
            outFile<<"#,"<<numRules<<endl;
            tempRuleOutputPtr.close();
            ifstream tempRuleOutputPtr2("./tempRuleOutput.csv");
            outFile<<tempRuleOutputPtr2.rdbuf();
            tempRuleOutputPtr2.close();
        }

        unsigned long int getTotalNodes() {
            if(maxPathLen[0]==2 and labels.size()==labels[0].size()+1)
                return labels.size()+((labels.size()-1)*(labels.size()-2))/2;
            else return labels.size();
        }

        void recordRecode(vector<itemRange>& record) {
            set<itemRange> newRecord;
            for(vector<itemRange>::iterator it=record.begin();it!=record.end();it++)
                if(posInSortedItems[*it+1]) newRecord.insert(posInSortedItems[*it+1]);
            record.clear();
            record.insert(record.end(),newRecord.begin(),newRecord.end());
        }
};

struct itemCompare {
    bool operator() (const vector<itemRange>& fRec, const vector<itemRange>& sRec) const {
        itemRange i,j,s1,s2;
        s1=fRec.size();
        s2=sRec.size();
        for(i=0,j=0; i<s1 and j<s2; i++, j++) {
            if(fRec[i]<sRec[j]) return true;
            else if(fRec[i]>sRec[j]) return false;
        }
        if(i==s1 and j!=s2) return true;
        else return false;
    }
};

class apriori {
    public:
        trie* Trie;
        vector<itemRange> record;
        maxNodes recordNum;
        map< vector<itemRange>, maxNodes, itemCompare> reducedRecords;
        int maxChildren=30;

        apriori() {
            Trie = new trie();
            recordNum=0;
        }

        void readRecord(FILE* inpFile) {
            set<itemRange> curRecord;
            char c;
            itemRange ptr;
            record.clear();

            do {
                itemRange item=0;
                ptr=0;
                c=getc(inpFile);
                while(c>='0' and c<='9') {
                    item*=10;
                    item+=c-'0';
                    c=getc(inpFile);
                    ptr++;
                }
                if(ptr) curRecord.insert(item);
            } while( c != '\n' && !feof(inpFile) );

            record.resize(curRecord.size());
            set<itemRange>::iterator it=curRecord.begin();
            for(int i=0; it!=curRecord.end(); it++, i++) {
                record[i]=*it;
            }
        }

        void calcSupport(FILE* inpFile, itemRange& candidateSize) {
            if(candidateSize==1) {
                recordNum=0;
                while(!feof(inpFile)) {
                    readRecord(inpFile);
                    if(!record.empty()) {
                        recordNum++;
                        Trie->findCandidate(record,candidateSize,0);
                    }
                }
            }
            else {
                if(candidateSize==2) {
                    while(!feof(inpFile)) {
                        readRecord(inpFile);
                        Trie->recordRecode(record);
                        if(record.size()>1) reducedRecords[record]++;
                    }
                }
                map< vector<itemRange>, maxNodes, itemCompare>::iterator it;
                for(it=reducedRecords.begin();it!=reducedRecords.end();it++) {
                    Trie->findCandidate(it->first,candidateSize,it->second);
                }

            }
        }

        void runApriori(ofstream& outFile, char* inpFileName, double minsup, double minconf, int flag) {
            FILE* inpFile=fopen(inpFileName,"r");
            itemRange candidateSize=1;
            calcSupport(inpFile,candidateSize);
            unsigned long int minCount=(unsigned long int)ceil(recordNum*minsup);
            Trie->deleteInfrequent(minCount);
            unsigned long int trieSize=Trie->getTotalNodes();
            Trie->genCandidates(candidateSize);
            while(trieSize<Trie->getTotalNodes()) {
                candidateSize++;
                fclose(inpFile);
                inpFile=fopen(inpFileName,"r");

                calcSupport(inpFile,candidateSize);

                Trie->deleteInfrequent(minCount);
                trieSize=Trie->getTotalNodes();
                Trie->genCandidates(candidateSize);
            }
            fclose(inpFile);
            Trie->genOutput(outFile);
            if(flag==1)
                Trie->mineRules(outFile,minconf);
        }
};

int main()
{
    double minsup,minconf;
    int pos,flag;
    string lin,param,val,inpFile,outFile;
    ifstream configFile;

    configFile.open("config.csv");

    while(configFile>>lin) {
        pos=lin.find(',');
        param=lin.substr(0,pos);
        val=lin.substr(pos+1);
        if(param=="input")
            inpFile=val;
        else if(param=="output")
            outFile=val;
        else if(param=="flag")
            flag=stoi(val);
        else if(param=="support")
            minsup=stod(val);
        else if(param=="confidence")
            minconf=stod(val);
    }

    configFile.close();

    inpFile="./converted.csv";
    ofstream outFilePtr("./tempOutput.csv");

    apriori aprioriInstance;
    char* inpFileName=new char[inpFile.size()+1];
    copy(inpFile.begin(),inpFile.end(),inpFileName);
    inpFileName[inpFile.size()]='\0';

    aprioriInstance.runApriori(outFilePtr,inpFileName,minsup,minconf,flag);
    outFilePtr.close();

    return 0;
}