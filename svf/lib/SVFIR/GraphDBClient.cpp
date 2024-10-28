#include "SVFIR/GraphDBClient.h"

using namespace SVF;

void GraphDBClient::loadSchema(neo4j_connection_t* connection, const std::string& filepath, const std::string& queryParam)
{
    if (connection != nullptr)
    {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            SVFUtil::outs() << "Failed to load Schema data from file: " << filepath << "\n";
            return;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        neo4j_value_t param = neo4j_string(buffer.str().c_str());

        neo4j_result_stream_t* result = neo4j_run(connection, queryParam.c_str(), param);

        if (NULL != result)
        {
            SVFUtil::outs()<<"Failed to write schema to db.\n";
        } else {
            SVFUtil::outs()<<"Schema wrote to db successfully.\n";
        }
    } else {
        SVFUtil::outs()<<"dbConnection is null.\n";
    }
}