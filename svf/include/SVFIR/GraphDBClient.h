#ifndef INCLUDE_GRAPHDBCLIENT_H_
#define INCLUDE_GRAPHDBCLIENT_H_
#include "Graphs/CallGraph.h"
#include "Graphs/ICFGEdge.h"
#include "Graphs/ICFGNode.h"
#include "SVFIR/SVFIR.h"
#include "SVFIR/SVFStatements.h"
#include "SVFIR/SVFType.h"
#include "Util/SVFUtil.h"
#include "Util/cJSON.h"
#include "lgraph/lgraph_rpc_client.h"
#include <errno.h>
#include <stdio.h>

namespace SVF
{
class RpcClient;
class ICFGEdge;
class ICFGNode;
class CallGraphEdge;
class CallGraphNode;
class SVFType;
class StInfo;
class SVFIR;
class SVFVar;
class SVFStmt;
class SVFBasicBlock;
class BasicBlockEdge;
class BasicBlockGraph;
class GraphDBClient
{
private:
    lgraph::RpcClient* connection;

    GraphDBClient()
    {
        const char* url = "127.0.0.1:9090";
        connection = new lgraph::RpcClient(url, "admin", "qazwsx123");
    }

    ~GraphDBClient()
    {
        if (connection != nullptr)
        {
            connection = nullptr;
        }
    }

public:
    static GraphDBClient& getInstance()
    {
        static GraphDBClient instance;
        return instance;
    }

    GraphDBClient(const GraphDBClient&) = delete;
    GraphDBClient& operator=(const GraphDBClient&) = delete;

    lgraph::RpcClient* getConnection()
    {
        return connection;
    }

    bool loadSchema(lgraph::RpcClient* connection, const std::string& filepath,
                    const std::string& dbname);
    bool createSubGraph(lgraph::RpcClient* connection,
                        const std::string& graphname);
    bool addCallGraphNode2db(lgraph::RpcClient* connection,
                             const CallGraphNode* node,
                             const std::string& dbname);
    bool addCallGraphEdge2db(lgraph::RpcClient* connection,
                             const CallGraphEdge* edge,
                             const std::string& dbname);
    bool addICFGNode2db(lgraph::RpcClient* connection, const ICFGNode* node,
                        const std::string& dbname);
    bool addICFGEdge2db(lgraph::RpcClient* connection, const ICFGEdge* edge,
                        const std::string& dbname);
    /// pasre the directcallsIds/indirectcallsIds string to vector
    std::vector<int> stringToIds(const std::string& str);

    void insertICFG2db(const ICFG* icfg);
    void insertCallGraph2db(const CallGraph* callGraph);
    void insertPAG2db(const SVFIR* pag);
    void insertBasicBlockGraph2db(const BasicBlockGraph* bbGraph);
    void insertSVFTypeNodeSet2db(const Set<const SVFType*>* types,
                                 const Set<const StInfo*>* stInfos,
                                 std::string& dbname);

    std::string getPAGNodeInsertStmt(const SVFVar* node);
    void insertPAGNode2db(lgraph::RpcClient* connection, const SVFVar* node,
                          const std::string& dbname);
    void insertPAGEdge2db(lgraph::RpcClient* connection, const SVFStmt* node,
                          const std::string& dbname);
    void insertBBNode2db(lgraph::RpcClient* connection,
                         const SVFBasicBlock* node, const std::string& dbname);
    void insertBBEdge2db(lgraph::RpcClient* connection,
                         const BasicBlockEdge* node, const std::string& dbname);
    std::string getPAGEdgeInsertStmt(const SVFStmt* edge);
    std::string getPAGNodeKindString(const SVFVar* node);

    /// parse ICFGNodes & generate the insert statement for ICFGNodes
    std::string getGlobalICFGNodeInsertStmt(const GlobalICFGNode* node);
    std::string getIntraICFGNodeInsertStmt(const IntraICFGNode* node);
    std::string getInterICFGNodeInsertStmt(const InterICFGNode* node);
    std::string getFunExitICFGNodeInsertStmt(const FunExitICFGNode* node);
    std::string getFunEntryICFGNodeInsertStmt(const FunEntryICFGNode* node);
    std::string getCallICFGNodeInsertStmt(const CallICFGNode* node);
    std::string getRetICFGNodeInsertStmt(const RetICFGNode* node);
    std::string getIntraCFGEdgeStmt(const IntraCFGEdge* edge);
    std::string getCallCFGEdgeStmt(const CallCFGEdge* edge);
    std::string getRetCFGEdgeStmt(const RetCFGEdge* edge);
    std::string getICFGNodeKindString(const ICFGNode* node);

    /// parse and generate the node insert statement for SVFType nodes
    std::string getSVFPointerTypeNodeInsertStmt(const SVFPointerType* node);
    std::string getSVFIntegerTypeNodeInsertStmt(const SVFIntegerType* node);
    std::string getSVFFunctionTypeNodeInsertStmt(const SVFFunctionType* node);
    std::string getSVFSturctTypeNodeInsertStmt(const SVFStructType* node);
    std::string getSVFArrayTypeNodeInsertStmt(const SVFArrayType* node);
    std::string getSVFOtherTypeNodeInsertStmt(const SVFOtherType* node);
    std::string getStInfoNodeInsertStmt(const StInfo* node);

    /// parse and generate the node insert statement for valvar nodes
    std::string getSVFVarNodeFieldsStmt(const SVFVar* node);
    std::string getValVarNodeFieldsStmt(const ValVar* node);
    std::string getValVarNodeInsertStmt(const ValVar* node);
    std::string getConstDataValVarNodeFieldsStmt(const ConstDataValVar* node);
    /// ConstDataValVar and its sub-class
    std::string getConstDataValVarNodeInsertStmt(const ConstDataValVar* node);
    std::string getBlackHoleValvarNodeInsertStmt(const BlackHoleValVar* node);
    std::string getConstFPValVarNodeInsertStmt(const ConstFPValVar* node);
    std::string getConstIntValVarNodeInsertStmt(const ConstIntValVar* node);
    std::string getConstNullPtrValVarNodeInsertStmt(
        const ConstNullPtrValVar* node);
    // parse and generate the node insert statement for valvar sub-class
    std::string getRetValPNNodeInsertStmt(const RetValPN* node);
    std::string getVarArgValPNNodeInsertStmt(const VarArgValPN* node);
    std::string getDummyValVarNodeInsertStmt(const DummyValVar* node);
    std::string getConstAggValVarNodeInsertStmt(const ConstAggValVar* node);
    std::string getGlobalValVarNodeInsertStmt(const GlobalValVar* node);
    std::string getFunValVarNodeInsertStmt(const FunValVar* node);
    std::string getGepValVarNodeInsertStmt(const GepValVar* node);
    std::string getArgValVarNodeInsertStmt(const ArgValVar* node);

    /// parse and generate the node insert statement for objvar nodes
    std::string getObjVarNodeFieldsStmt(const ObjVar* node);
    std::string getObjVarNodeInsertStmt(const ObjVar* node);
    std::string getBaseObjVarNodeFieldsStmt(const BaseObjVar* node);
    std::string getBaseObjNodeInsertStmt(const BaseObjVar* node);
    std::string getGepObjVarNodeInsertStmt(const GepObjVar* node);

    /// parse and generate the node insert statement for baseObjVar sub-class
    std::string getHeapObjVarNodeInsertStmt(const HeapObjVar* node);
    std::string getStackObjVarNodeInsertStmt(const StackObjVar* node);
    std::string getConstDataObjVarNodeFieldsStmt(const ConstDataObjVar* node);
    std::string getConstDataObjVarNodeInsertStmt(const ConstDataObjVar* node);
    std::string getConstNullPtrObjVarNodeInsertStmt(
        const ConstNullPtrObjVar* node);
    std::string getConstIntObjVarNodeInsertStmt(const ConstIntObjVar* node);
    std::string getConstFPObjVarNodeInsertStmt(const ConstFPObjVar* node);
    std::string getDummyObjVarNodeInsertStmt(const DummyObjVar* node);
    std::string getConstAggObjVarNodeInsertStmt(const ConstAggObjVar* node);
    std::string getGlobalObjVarNodeInsertStmt(const GlobalObjVar* node);
    std::string getFunObjVarNodeInsertStmt(const FunObjVar* node);

    /// parse and generate the edge insert statement for SVFStmt
    std::string generateSVFStmtEdgeFieldsStmt(const SVFStmt* edge);
    std::string generateSVFStmtEdgeInsertStmt(const SVFStmt* edge);
    /// parse and generate the edge insert statement for AssignStmt & its
    /// sub-class
    std::string generateAssignStmtFieldsStmt(const AssignStmt* edge);
    std::string generateAssignStmtEdgeInsertStmt(const AssignStmt* edge);
    std::string generateAddrStmtEdgeInsertStmt(const AddrStmt* edge);
    std::string generateCopyStmtEdgeInsertStmt(const CopyStmt* edge);
    std::string generateStoreStmtEdgeInsertStmt(const StoreStmt* edge);
    std::string generateLoadStmtEdgeInsertStmt(const LoadStmt* edge);
    std::string generateGepStmtEdgeInsertStmt(const GepStmt* edge);
    std::string generateCallPEEdgeInsertStmt(const CallPE* edge);
    std::string generateRetPEEdgeInsertStmt(const RetPE* edge);
    std::string generateTDForkPEEdgeInsertStmt(const TDForkPE* edge);
    std::string generateTDJoinPEEdgeInsertStmt(const TDJoinPE* edge);
    /// parse and generate the edge insert statement for MultiOpndStmt & its
    /// sub-class
    std::string generateMultiOpndStmtEdgeFieldsStmt(const MultiOpndStmt* edge);
    std::string generateMultiOpndStmtEdgeInsertStmt(const MultiOpndStmt* edge);
    std::string generatePhiStmtEdgeInsertStmt(const PhiStmt* edge);
    std::string generateSelectStmtEndgeInsertStmt(const SelectStmt* edge);
    std::string generateCmpStmtEdgeInsertStmt(const CmpStmt* edge);
    std::string generateBinaryOPStmtEdgeInsertStmt(const BinaryOPStmt* edge);
    std::string genereateUnaryOPStmtEdgeInsertStmt(const UnaryOPStmt* edge);
    std::string generateBranchStmtEdgeInsertStmt(const BranchStmt* edge);

    /// parse and generate the insert statement for BasicBlockGraph
    std::string getBBNodeInsertStmt(const SVFBasicBlock* node);
    std::string getBBEdgeInsertStmt(const BasicBlockEdge* edge);


    cJSON* queryFromDB(lgraph::RpcClient* connection, const std::string& dbname, std::string queryStatement);
    /// read SVFType from DB
    void readSVFTypesFromDB(lgraph::RpcClient* connection,
                            const std::string& dbname, SVFIR* pag);
    void addSVFTypeNodeFromDB(lgraph::RpcClient* connection,
                               const std::string& dbname, SVFIR* pag);

    /// read BasicBlockGraph from DB
    void readBasicBlockGraphFromDB(lgraph::RpcClient* connection, const std::string& dbname);
    void readBasicBlockNodesFromDB(lgraph::RpcClient* connection, const std::string& dbname, FunObjVar* funObjVar);
    void readBasicBlockEdgesFromDB(lgraph::RpcClient* connection, const std::string& dbname, FunObjVar* funObjVar);

    /// read ICFGNodes & ICFGEdge from DB
    ICFG* buildICFGFromDB(lgraph::RpcClient* connection, const std::string& dbname, SVFIR* pag);
    /// ICFGNodes
    void readICFGNodesFromDB(lgraph::RpcClient* connection, const std::string& dbname, std::string nodeType, ICFG* icfg, SVFIR* pag);
    ICFGNode* parseGlobalICFGNodeFromDBResult(const cJSON* node);
    ICFGNode* parseFunEntryICFGNodeFromDBResult(const cJSON* node, SVFIR* pag);
    ICFGNode* parseFunExitICFGNodeFromDBResult(const cJSON* node, SVFIR* pag);
    ICFGNode* parseRetICFGNodeFromDBResult(const cJSON* node, SVFIR* pag);
    ICFGNode* parseIntraICFGNodeFromDBResult(const cJSON* node, SVFIR* pag);
    ICFGNode* parseCallICFGNodeFromDBResult(const cJSON* node, SVFIR* pag);
    /// ICFGEdges
    void readICFGEdgesFromDB(lgraph::RpcClient* connection, const std::string& dbname, std::string edgeType, ICFG* icfg, SVFIR* pag);
    ICFGEdge* parseIntraCFGEdgeFromDBResult(const cJSON* edge, SVFIR* pag, ICFG* icfg);
    ICFGEdge* parseCallCFGEdgeFromDBResult(const cJSON* edge, SVFIR* pag, ICFG* icfg);
    ICFGEdge* parseRetCFGEdgeFromDBResult(const cJSON* edge, SVFIR* pag, ICFG* icfg);

    //// read CallGraph Nodes & CallGraphEdge from DB
    // void readCallGraphNodesFromDB(lgraph::RpcClient* connection, const std::string& dbname, SVFIR* pag);
    // void readCallGraphEdgesFromDB(lgraph::RpcClient* connection, const std::string& dbname, SVFIR* pag);

    /// read PAGNodes from DB
    void readPAGNodesFromDB(lgraph::RpcClient* connection, const std::string& dbname, std::string nodeType, SVFIR* pag);
    void initialSVFPAGNodesFromDB(lgraph::RpcClient* connection, const std::string& dbname, SVFIR* pag);
    ObjTypeInfo* parseObjTypeInfoFromDB(cJSON* properties, SVFIR* pag);

    template <typename Container>
    std::string extractNodesIds(const Container& nodes)
    {
        if (nodes.empty())
        {
            return "";
        }
        std::ostringstream nodesIds;
        auto it = nodes.begin();

        nodesIds << (*it)->getId();
        ++it;

        for (; it != nodes.end(); ++it)
        {
            nodesIds << "," << (*it)->getId();
        }

        return nodesIds.str();
    }

    template <typename Container>
    std::string extractEdgesIds(const Container& edges)
    {
        if (edges.empty())
        {
            return "";
        }
        std::ostringstream edgesIds;
        auto it = edges.begin();

        edgesIds << (*it)->getEdgeID();
        ++it;

        for (; it != edges.end(); ++it)
        {
            edgesIds << "," << (*it)->getEdgeID();
        }

        return edgesIds.str();
    }

    template <typename Container>
    std::string extractIdxs(const Container& idxVec)
    {
        if (idxVec.empty())
        {
            return "";
        }
        std::ostringstream idxVecStr;
        auto it = idxVec.begin();
    
        idxVecStr << *it;
        ++it;
    
        for (; it != idxVec.end(); ++it)
        {
            idxVecStr << "," << *it;
        }
    
        return idxVecStr.str();
    }

    template <typename Container>
    Container parseElements2Container(std::string& str)
    {
        str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
        str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
        Container idxVec;
        std::istringstream ss(str);
        std::string token;

        while (std::getline(ss, token, ','))
        {
            if constexpr (std::is_same<typename Container::value_type,
                                       int>::value)
            {
                idxVec.push_back(std::stoi(token));
            }
            else if constexpr (std::is_same<typename Container::value_type,
                                            uint32_t>::value)
            {
                idxVec.push_back(static_cast<uint32_t>(std::stoul(token)));
            }
            else
            {
                idxVec.push_back(token);
            }
        }

        return idxVec;
    }

    template <typename Container>
    std::string extractSVFTypes(const Container& types)
    {
        if (types.empty())
        {
            return "";
        }
    
        std::ostringstream typesStr;
        auto it = types.begin();
    
        
        typesStr << "{" << (*it)->toString() << "}";
        ++it;
    
        for (; it != types.end(); ++it)
        {
            typesStr << "," << "{" << (*it)->toString() << "}";
        }
    
        return typesStr.str();
    }

    template <typename Container>
    Container parseElementsToSVFTypeContainer(
        std::string& str, const Map<std::string, SVFType*>& typeMap)
    {
        str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
        str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    
        Container resultContainer;
    
        size_t pos = 0;
        while ((pos = str.find('{')) != std::string::npos)
        {
            size_t endPos = str.find('}', pos);
            if (endPos == std::string::npos)
            {
                SVFUtil::outs() << "Warning: Mismatched brackets in input string\n";
                break;
            }
    
            std::string token = str.substr(pos + 1, endPos - pos - 1);
            
            token.erase(token.find_last_not_of(" \t") + 1);
            token.erase(0, token.find_first_not_of(" \t"));
    
            auto it = typeMap.find(token);
            if (it != typeMap.end())
            {
                resultContainer.insert(resultContainer.end(), it->second);
            }
            else
            {
                SVFUtil::outs()
                    << "Warning: No matching SVFType found for token '" << token
                    << "'\n";
            }
    
            str = str.substr(endPos + 1);
        }
    
        return resultContainer;
    }

    Set<std::string> parseSVFTypes(std::string& typesStr)
    {
        typesStr.erase(std::remove(typesStr.begin(), typesStr.end(), '\n'),
                       typesStr.end());
        typesStr.erase(std::remove(typesStr.begin(), typesStr.end(), '\r'),
                       typesStr.end());
        Set<std::string> result;
        std::stringstream ss(typesStr);
        std::string token;

        while (std::getline(ss, token, ','))
        {
            result.insert(token);
        }

        return result;
    }

    template <typename MapType>
    std::string extractFldIdx2TypeMap(const MapType& fldIdx2TypeMap)
    {
        if (fldIdx2TypeMap.empty())
        {
            return "";
        }
        
        std::ostringstream mapStr;
        auto it = fldIdx2TypeMap.begin();
    
        mapStr << "{" << it->first << ":" << it->second->toString() << "}";
        ++it;
    
        for (; it != fldIdx2TypeMap.end(); ++it)
        {
            mapStr << ",{" << it->first << ":" << it->second->toString() << "}";
        }
    
        return mapStr.str();
    }

    template <typename MapType>
    MapType parseStringToFldIdx2TypeMap(const std::string& str, const Map<std::string, SVFType*>& typeMap)
    {
        MapType resultMap;
    
        std::string cleanedStr = str;
        cleanedStr.erase(0, cleanedStr.find_first_not_of(" \t"));
        cleanedStr.erase(cleanedStr.find_last_not_of(" \t") + 1);
    
        size_t startPos = 0;
        size_t openBracePos = cleanedStr.find('{', startPos);
    
        while (openBracePos != std::string::npos) {
            size_t closeBracePos = cleanedStr.find('}', openBracePos);
            if (closeBracePos == std::string::npos) {
                break;  
            }
    
            std::string token = cleanedStr.substr(openBracePos + 1, closeBracePos - openBracePos - 1);
    
            size_t colonPos = token.find(':');
            if (colonPos != std::string::npos) {
                std::string keyStr = token.substr(0, colonPos);
                std::string typeStr = token.substr(colonPos + 1);

                u32_t key = static_cast<u32_t>(std::stoi(keyStr));
    
                auto it = typeMap.find(typeStr);
                if (it != typeMap.end()) {
                    resultMap[key] = it->second;
                } else {
                    SVFUtil::outs() << "Warning: No matching SVFType found for type: " << typeStr << "\n";
                }
            }
    
            startPos = closeBracePos + 1;
            openBracePos = cleanedStr.find('{', startPos);
        }
    
        return resultMap;
    }

    template <typename LabelMapType>
    std::string extractLabelMap2String(const LabelMapType& labelMap)
    {
        if (labelMap->empty())
        {
            return "";
        }
        std::ostringstream mapStr;

        for (auto it = labelMap->begin(); it != labelMap->end(); ++it)
        {
            if (it != labelMap->begin())
            {
                mapStr << ",";
            }
            mapStr << (it->first ? std::to_string(it->first->getId()) : "NULL")
                   << ":" << std::to_string(it->second);
        }

        return mapStr.str();
    }

    template <typename BBsMapWithSetType>
    std::string extractBBsMapWithSet2String(const BBsMapWithSetType& bbsMap)
    {
        if (bbsMap->empty())
        {
            return "";
        }
        std::ostringstream mapStr;
        auto it = bbsMap->begin();

        for (; it != bbsMap->end(); ++it)
        {
            mapStr << "[" << it->first->getId() << ":";
            mapStr << extractNodesIds(it->second);
            mapStr << "]";
        }

        return mapStr.str();
    }

    template <typename BBsMapType>
    std::string extractBBsMap2String(const BBsMapType& bbsMap)
    {
        if (bbsMap->empty())
        {
            return "";
        }
        std::ostringstream mapStr;
        for (const auto& pair : *bbsMap)
        {
            if (mapStr.tellp() != std::streampos(0))
            {
                mapStr << ",";
            }
            if (pair.first != nullptr && pair.second != nullptr)
            {
                mapStr << pair.first->getId() << ":" << pair.second->getId();
            }
            else if (pair.first == nullptr)
            {
                mapStr << "NULL:" << pair.second->getId();
            }
            else if (pair.second == nullptr)
            {
                mapStr << pair.first->getId() << ":NULL";
            }
        }
        return mapStr.str();
    }

    template <typename MapType>
    std::string pagEdgeToSetMapTyToString(const MapType& map)
    {
        if (map.empty())
        {
            return "";
        }

        std::ostringstream oss;

        for (auto it = map.begin(); it != map.end(); ++it)
        {
            oss << "[" << it->first << ":";
            const SVFStmt::SVFStmtSetTy& set = it->second;
            for (auto setIt = set.begin(); setIt != set.end(); ++setIt)
            {
                if (setIt != set.begin())
                {
                    oss << ",";
                }
                oss << (*setIt)->getEdgeID();
            }
            oss << "]";

            if (std::next(it) != map.end())
            {
                oss << ",";
            }
        }

        return oss.str();
    }

    /// Convert IdxOperandPairs to string
    std::string IdxOperandPairsToString(
        const AccessPath::IdxOperandPairs* idxOperandPairs) const
    {
        if (idxOperandPairs->empty())
        {
            return "";
        }

        std::ostringstream oss;
        oss << "[";
        for (auto it = idxOperandPairs->begin(); it != idxOperandPairs->end();
             ++it)
        {
            if (it != idxOperandPairs->begin())
            {
                oss << ", ";
            }
            oss << IdxOperandPairToString(*it);
        }
        oss << "]";
        return oss.str();
    }

    std::string IdxOperandPairToString(
        const AccessPath::IdxOperandPair& pair) const
    {
        std::ostringstream oss;
        if (nullptr != pair.first && nullptr != pair.second)
        {
            oss << "{" << pair.first->getId() << ", " << pair.second->toString()
                << "}";
            return oss.str();
        }
        else if (nullptr == pair.second)
        {
            oss << "{" << pair.first->getId() << ", NULL}";
        }
        return "";
    }

    std::string extractSuccessorsPairSet2String(
        const BranchStmt::SuccAndCondPairVec* vec)
    {
        std::ostringstream oss;
        for (auto it = vec->begin(); it != vec->end(); ++it)
        {
            if (it != vec->begin())
            {
                oss << ",";
            }
            oss << (*it).first->getId() << ":" << std::to_string((*it).second);
        }

        return oss.str();
    }

    int parseBBId(const std::string& str) {
        size_t pos = str.find(':');
        int number = std::stoi(str.substr(0, pos));
        return number;
    }
};

} // namespace SVF

#endif