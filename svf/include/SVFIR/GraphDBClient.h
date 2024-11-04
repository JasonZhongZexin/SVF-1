#ifndef INCLUDE_GRAPHDBCLIENT_H_
#define INCLUDE_GRAPHDBCLIENT_H_
#include "Util/SVFUtil.h"
#include "lgraph/lgraph_rpc_client.h"
#include <errno.h>
#include <stdio.h>
#include "Graphs/CallGraph.h"

namespace SVF
{
class RpcClient;
class CallGraphEdge;
class CallGraphNode;
class GraphDBClient
{
private:
    lgraph::RpcClient* connection;

    GraphDBClient()
    {
        const char* url = "127.0.0.1:9090";
        connection = new lgraph::RpcClient(url, "admin", "73@TuGraph");
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
    bool addCallGraphNode2db(lgraph::RpcClient* connection, const CallGraphNode* node,
                    const std::string& dbname);
    bool addCallGraphEdge2db(lgraph::RpcClient* connection,
                             const CallGraphEdge* edge, const int& csid,
                             const std::string& dbname);
    // pasre the directcallsIds/indirectcallsIds string to vector
    std::vector<int> stringToIds(const std::string& str);
};

} // namespace SVF

#endif