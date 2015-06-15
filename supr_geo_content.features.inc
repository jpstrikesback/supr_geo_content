<?php
/**
 * @file
 * supr_geo_content.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function supr_geo_content_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function supr_geo_content_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function supr_geo_content_node_info() {
  $items = array(
    'listing' => array(
      'name' => t('Listing'),
      'base' => 'node_content',
      'description' => t('A Classified/Other Geo Tagged Listing.'),
      'has_title' => '1',
      'title_label' => t('Listing Name'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}

/**
 * Implements hook_default_wsclient_service().
 */
function supr_geo_content_default_wsclient_service() {
  $items = array();
  $items['geonames_country_subdivision'] = entity_import('wsclient_service', '{
    "settings" : [],
    "operations" : { "get_country_subdivision" : {
        "url" : "",
        "label" : "Get Country Subdivision",
        "parameter" : {
          "lat" : { "type" : "decimal" },
          "lng" : { "type" : "decimal" },
          "lang" : { "type" : "text", "default value" : "en" },
          "username" : { "type" : "text", "default value" : "demo" },
          "maxRows" : { "type" : "text", "default value" : "10" },
          "radius" : { "type" : "text", "default value" : "100" }
        },
        "name" : "get_country_subdivision",
        "result" : {
          "type" : "result_data_country_subdivision",
          "label" : "Country Subdivision Result"
        }
      }
    },
    "datatypes" : {
      "result_country_subdivision" : {
        "label" : "Country Subdivision result",
        "property info" : { "geonames" : { "type" : "result_data_country_subdivision", "label" : "" } },
        "name" : "result_country_subdivision"
      },
      "country_subdivision_codes_result" : {
        "label" : "Country Subdivision Codes Result",
        "property info" : { "codes" : { "type" : "list\\u003Ccountry_subdivision_codes_result_data\\u003E" } },
        "name" : "country_subdivision_codes_result"
      },
      "country_subdivision_codes_result_data" : {
        "label" : "Country Subdivision Codes Result Data",
        "property info" : {
          "code" : { "type" : "text", "label" : "code" },
          "type" : { "type" : "text", "label" : "type" }
        },
        "name" : "country_subdivision_codes_result_data"
      },
      "result_data_country_subdivision" : {
        "label" : "Country Subdivision Result Data",
        "property info" : {
          "distance" : { "type" : "text", "label" : "distance" },
          "adminCode1" : { "type" : "text", "label" : "adminCode1" },
          "countryName" : { "type" : "text", "label" : "countryName" },
          "countryCode" : { "type" : "text", "label" : "countryCode" },
          "codes" : { "type" : "list\\u003Ccountry_subdivision_codes_result_data\\u003E" },
          "adminName1" : { "type" : "text", "label" : "adminName1" }
        },
        "name" : "result_data_country_subdivision"
      }
    },
    "global_parameters" : [],
    "name" : "geonames_country_subdivision",
    "label" : "Geonames Country Subdivision",
    "url" : "http:\\/\\/api.geonames.org\\/countrySubdivisionJSON",
    "type" : "rest",
    "token" : "OH9xJSS0mSm_O5KR7SOtO_bArUByoAXrkyCpSyry7uE",
    "authentication" : null,
    "rdf_mapping" : []
  }');
  $items['geonames_nearby_cities'] = entity_import('wsclient_service', '{
    "settings" : [],
    "operations" : { "get_nearby_cities" : {
        "url" : "",
        "label" : "Get Nearby Cities",
        "parameter" : {
          "north" : { "type" : "decimal" },
          "south" : { "type" : "decimal" },
          "east" : { "type" : "decimal" },
          "west" : { "type" : "decimal" },
          "lang" : { "type" : "text", "default value" : "en" },
          "username" : { "type" : "text", "default value" : "demo" },
          "maxRows" : { "type" : "integer", "default value" : "10" }
        },
        "name" : "get_nearby_cities",
        "result" : { "type" : "nearby_cities_result", "label" : "Nearby Cities Result" }
      }
    },
    "datatypes" : {
      "nearby_cities_result" : {
        "label" : "Nearby Cities result",
        "property info" : { "geonames" : { "type" : "list\\u003Cresult_data\\u003E" } },
        "name" : "nearby_cities_result"
      },
      "result_data" : {
        "label" : "Nearby Citiies Result Data",
        "property info" : {
          "fcodeName" : { "type" : "text", "label" : "fcodeName" },
          "toponymName" : { "type" : "text", "label" : "toponymName" },
          "countrycode" : { "type" : "text", "label" : "countrycode" },
          "fcl" : { "type" : "text", "label" : "fcl" },
          "fclName" : { "type" : "text", "label" : "fclName" },
          "name" : { "type" : "text", "label" : "name" },
          "wikipedia" : { "type" : "text", "label" : "wikipedia" },
          "lng" : { "type" : "decimal", "label" : "longitude" },
          "fcode" : { "type" : "text", "label" : "fcode" },
          "geonameId" : { "type" : "integer", "label" : "geonameId" },
          "lat" : { "type" : "decimal", "label" : "latitude" },
          "population" : { "type" : "integer", "label" : "population" }
        },
        "name" : "result_data"
      }
    },
    "global_parameters" : [],
    "name" : "geonames_nearby_cities",
    "label" : "Geonames nearby cities",
    "url" : "http:\\/\\/api.geonames.org\\/citiesJSON",
    "type" : "rest",
    "token" : "OH9xJSS0mSm_O5KR7SOtO_bArUByoAXrkyCpSyry7uE",
    "authentication" : null,
    "rdf_mapping" : []
  }');
  return $items;
}
