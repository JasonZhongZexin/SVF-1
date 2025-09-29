#include "Graphs/BasicBlockG.h"
#include "Graphs/ICFGNode.h"
#include "SVFIR/GraphDBClient.h"
#include "Util/Options.h"

using namespace SVF;
const std::string BasicBlockEdge::toString() const
{
    std::string str;
    std::stringstream rawstr(str);
    rawstr << "BasicBlockEdge: " << getSrcNode()->toString() << " -> " << getDstNode()->getName();
    return rawstr.str();
}

const std::string SVFBasicBlock::toString() const
{
    std::string str;
    std::stringstream rawstr(str);
    rawstr << "----------"<< "SVFBasicBlock: " << getName() <<"----------\n";
    for (const ICFGNode* icfgNode : allICFGNodes)
    {
        rawstr << icfgNode->toString();
    }
    rawstr << "\n----------------------------------------\n";
    return rawstr.str();
}

SVFBasicBlock* BasicBlockGraph::addBasicBlock(const std::string& bbname)
    {   
        id++;
        int nodeId = id;
        int extID = GraphDBClient::getInstance().getExternalID();
        if (Options::ReadFromDB() && extID != -1)
        {
            nodeId = extID;
        } 
        SVFBasicBlock* bb = new SVFBasicBlock(nodeId, nullptr);
        addGNode(nodeId, bb);
        bb->setName(bbname);
        return bb;
    }
