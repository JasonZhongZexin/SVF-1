#ifndef INCLUDE_GRAPHDBCLIENT_H_
#define INCLUDE_GRAPHDBCLIENT_H_
#include <neo4j-client.h>
#include "Util/SVFUtil.h"


namespace SVF
{
    class GraphDBClient{
        private:
            neo4j_connection_t* connection;

            GraphDBClient()
            {
                const char* uri = "bolt://localhost:7687";
                neo4j_config_t* config = neo4j_new_config();
                if (neo4j_config_set_username(config, "admin") != 0 && 
                    neo4j_config_set_password(config, "73@TuGraph") != 0){
                     SVFUtil::outs() << "Failed to connect to GraphDB.\n";
                } else {
                    connection = neo4j_connect(uri, config, NEO4J_INSECURE);
                    if (connection == nullptr) {
                        SVFUtil::outs() << "Failed to connect to GraphDB.\n";
                    } else {
                        SVFUtil::outs() << "Connected to GraphDB successfully.\n";
                    }
                }
                
            }

            ~GraphDBClient()
            {
                if (connection != nullptr)
                {
                   if (neo4j_close(connection) !=0){
                        SVFUtil::outs() << "Failed to close GraphDB.\n";
                    } else {
                        SVFUtil::outs() << "GraphDB connection closed.\n";
                        connection = nullptr; 
                    }
                }
            }
        public:
            static GraphDBClient& getInstance()
            {
                static GraphDBClient instance;
                return instance;
            }

            GraphDBClient(const GraphDBClient&) = delete;
            GraphDBClient& operator = (const GraphDBClient&) = delete;

            neo4j_connection_t* getConnection()
            {
                return connection;
            }
            
            void loadSchema(neo4j_connection_t* connection, const std::string& filepath, const std::string& queryParam);
    };

}

#endif 