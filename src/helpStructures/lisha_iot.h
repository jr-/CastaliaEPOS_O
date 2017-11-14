//#define LISHA_IOT
#ifdef LISHA_IOT

#include <cassandra.h>
#include <iostream>
#include <sstream>
#include <list>

class LISHA_IoT_Connector {
private:
    typedef struct {
        const cass_byte_t* bytes;
        size_t length;
    } cass_byte_t_w_size;

public:
    /*typedef struct{
     cass_int64_t timestamp;
     cass_double_t value;
     } Entry;*/

    bool has_data = false;

public:

    LISHA_IoT_Connector(const char* series, unsigned pg_size = 100) {
        _paging_size = pg_size;

        /* Setup and connect to cluster */
        _cluster = cass_cluster_new();
        _session = cass_session_new();

        /* Add contact points */
        cass_cluster_set_contact_points(_cluster, "150.162.62.3");
        cass_cluster_set_credentials(_cluster, "omnet", "omnet@iot:2017");
        /* Provide the cluster object as configuration to connect the session */
        CassFuture* connect_future = cass_session_connect_keyspace(_session,
                _cluster, "kairosdb");

        if (cass_future_error_code(connect_future) == CASS_OK) {
            _close_future = NULL;

            //_statement_data_point =
            query_row_key_index(series);

            _has_more_pages = cass_true;

            query_data_point();
        }
    }

    ~LISHA_IoT_Connector() {
        //cass_iterator_free(_iterator_data_point);
        cass_result_free(_result_data_point);
        cass_future_free(_future_data_point);
        cass_statement_free(_statement_data_point);

        /* Close the session */
        _close_future = cass_session_close(_session);
        cass_future_wait(_close_future);
        cass_future_free(_close_future);

        cass_cluster_free(_cluster);
        cass_session_free(_session);
    }

    double value(unsigned long long timestamp = 0) {

        if (cass_iterator_next(_iterator_data_point)) {
            const CassRow* row = cass_iterator_get_row(_iterator_data_point);
            /* Retreive and use values from the row */
            const CassValue* column_value = cass_row_get_column_by_name(row,
                    "value");
            //const CassValue* column_column1 = cass_row_get_column_by_name(row, "column1");

            cass_double_t value;
            cass_value_get_double(column_value, &value);

            //cass_int64_t timestamp;
            //cass_value_get_int64(column_column1, &timestamp);

            return value;
        } else {
            cass_iterator_free(_iterator_data_point);

            /* Check to see if there are more pages remaining for this result */
            _has_more_pages = cass_result_has_more_pages(_result_data_point);

            if (_has_more_pages) {
                /* If there are more pages we need to set the position for the next execute */
                cass_statement_set_paging_state(_statement_data_point,
                        _result_data_point);

                cass_result_free(_result_data_point);
                query_data_point();

                return value();
            } else {
                has_data = false;
                return 0;
            }
        }
    }

private:

    //CassStatement *
    void query_row_key_index(const char* series) {
        //CassStatement* statement_data_point;

        const char* query =
                "SELECT key, column1 FROM row_key_index WHERE key = textAsBlob(?);";

        CassStatement* statement = cass_statement_new(query, 1);
        cass_statement_bind_string(statement, 0, series);

        CassFuture* future = cass_session_execute(_session, statement);

        if (cass_future_error_code(future) == CASS_OK) {
            const CassResult* result = cass_future_get_result(future);
            CassIterator* iterator_row_key_index = cass_iterator_from_result(
                    result);

            std::list<cass_byte_t_w_size> keys_data_points;

            while (cass_iterator_next(iterator_row_key_index)) {
                const CassRow* row = cass_iterator_get_row(
                        iterator_row_key_index);
                /* Retreive and use values from the row */
                const CassValue* column1 = cass_row_get_column_by_name(row,
                        "column1");

                const cass_byte_t* column1_bytes;
                size_t column1_bytes_length;
                cass_value_get_bytes(column1, &column1_bytes,
                        &column1_bytes_length);

                keys_data_points.push_back( { column1_bytes,
                        column1_bytes_length });

            }

            cass_iterator_free(iterator_row_key_index);

            std::string bind_pattern = "";

            bool first = true;
            for (int i = 0; i < keys_data_points.size(); i++) {
                if (first) {
                    bind_pattern += "?";
                    first = false;
                } else {
                    bind_pattern += " , ?";
                }

            }

            std::string query_return_tmp =
                    "SELECT * FROM data_points WHERE key IN(" + bind_pattern
                            + ");";
            const char* query_return = query_return_tmp.c_str();

            _statement_data_point = cass_statement_new(query_return,
                    keys_data_points.size());
            cass_statement_set_paging_size(_statement_data_point, _paging_size);

            int i = 0;
            for (std::list<cass_byte_t_w_size>::iterator it =
                    keys_data_points.begin(); it != keys_data_points.end();
                    it++) {
                cass_statement_bind_bytes(_statement_data_point, i++,
                        (*it).bytes, (*it).length);

            }

            cass_result_free(result);

        } else {
            /* Handle error */
            const char* message;
            size_t message_length;
            cass_future_error_message(future, &message, &message_length);
            fprintf(stderr, "Unable to run query: '%.*s'\n",
                    (int) message_length, message);
        }

        cass_statement_free(statement);
        cass_future_free(future);
    }

    bool query_data_point() {
        ///////////////////////////////////////////////
        //_has_more_pages = cass_true;
        //std::cout << "query_data_point" << std::endl;

        if (_has_more_pages) {
            _future_data_point = cass_session_execute(_session,
                    _statement_data_point);

            _result_data_point = cass_future_get_result(_future_data_point);

            if (_result_data_point == NULL) {
                std::cout << "/* Handle error */" << std::endl;
                cass_future_free(_future_data_point);
                return false;
            }

            /* Get values from result... */
            _iterator_data_point = cass_iterator_from_result(
                    _result_data_point);

        } else {
            return false;
        }

        has_data = true;
        return true;
    }

private:
    CassCluster* _cluster = 0;
    CassSession* _session = 0;
    CassFuture* _close_future = 0;
    unsigned int _paging_size;

    //////////////////////////////////////

    CassFuture * _future_data_point = 0;
    const CassResult * _result_data_point = 0;
    CassIterator * _iterator_data_point = 0;
    CassStatement * _statement_data_point = 0;
    cass_bool_t _has_more_pages = cass_false;

};

#else

class LISHA_IoT_Connector
{
public:

    LISHA_IoT_Connector(const char* series, unsigned pg_size = 100) {}

    ~LISHA_IoT_Connector() {}

    double value(unsigned long long timestamp = 0) {
        static int ret = 0;
        return ret++;
    }
};

#endif
