#include "SVFIR/GraphDBClient.h"

using namespace SVF;

bool GraphDBClient::loadSchema(lgraph::RpcClient* connection,
                               const std::string& filepath,
                               const std::string& dbname)
{
    if (nullptr != connection)
    {
        std::string result;
        bool ret =
            connection->ImportSchemaFromFile(result, filepath, "callGraph");
        SVFUtil::outs() << "Schema Loading Results: " << result << "\n";
        return ret;
    }
    return false;
}

bool GraphDBClient::addCallGraphNode2db(lgraph::RpcClient* connection,
                                        const CallGraphNode* node,
                                        const std::string& dbname)
{
    if (nullptr != connection)
    {
        std::string is_reachable_str = node->isReachableFromProgEntry() ? "true" : "false";
        const std::string queryStatement ="CREATE (n:CallGraphNode {id: " + std::to_string(node->getId()) +
                                 ", function_name: '" + node->getFunction()->getName().c_str() +
                                 "', is_reachable_from_prog_entry: " + is_reachable_str + "})";
        SVFUtil::outs()<<"query:"<<queryStatement<<"\n";
        std::string result;
        bool ret = connection->CallCypher(result, queryStatement, dbname);
        if (ret)
        {
            SVFUtil::outs() << result << "\n";
        }
        else
        {
            SVFUtil::outs() << "Failed to add node to db " << dbname << " "
                            << result << "\n";
        }
        return ret;
    }
    return false;
}

bool GraphDBClient::addCallGraphEdge2db(lgraph::RpcClient* connection,
                                        const CallGraphEdge* edge,
                                        const int& csid,
                                        const std::string& dbname)
{
    if (nullptr != connection)
    {
        std::ostringstream indirectCallIds;
        Set<const CallICFGNode*> indirectCall = edge->getIndirectCalls();
        if (indirectCall.size() > 0)
        {
            size_t i = 0;
            for (const CallICFGNode* node : indirectCall)
            {
                indirectCallIds << node->getId();
                if (i < indirectCall.size() - 1)
                {
                    indirectCallIds << ",";
                }
                ++i;
            }
        }
        else
        {
            indirectCallIds << "";
        }

        std::ostringstream directCallIds;
        Set<const CallICFGNode*> directCall = edge->getDirectCalls();
        if (directCall.size() > 0)
        {
            size_t i = 0;
            for (const CallICFGNode* node : directCall)
            {
                directCallIds << node->getId();
                if (i < directCall.size() - 1)
                {
                    directCallIds << ",";
                }
                ++i;
            }
        }
        else
        {
            directCallIds << "";
        }

        const std::string queryStatement =
            "MATCH (n:CallGraphNode), (m:CallGraphNode) WHERE n.id = " +
            std::to_string(edge->getSrcNode()->getId()) +
            " AND m.id = " + std::to_string(edge->getDstNode()->getId()) +
            "CREATE (n)-[r:CallRetEdge{csid:" + std::to_string(csid) +
            ", kind:" + std::to_string(edge->getEdgeKind()) +
            ", isDirectCall:" + std::to_string(edge->isDirectCallEdge()) +
            ", directCalls:'" + directCallIds.str() + "', indirectCalls:'" +
            indirectCallIds.str() + "'}]->(m)";
        SVFUtil::outs() << "query:" << queryStatement << "\n";
        std::string result;
        bool ret = connection->CallCypher(result, queryStatement, dbname);
        if (ret)
        {
            SVFUtil::outs() << result << "\n";
        }
        else
        {
            SVFUtil::outs() << "Failed to add edges to db " << dbname << " "
                            << result << "\n";
        }
        return ret;
    }
    return false;
}

// pasre the directcallsIds/indirectcallsIds string to vector
std::vector<int> GraphDBClient::stringToIds(const std::string& str)
{
    std::vector<int> ids;
    std::istringstream iss(str);
    std::string token;
    while (std::getline(iss, token, ','))
    {
        ids.push_back(std::stoi(token));
    }
    return ids;
}