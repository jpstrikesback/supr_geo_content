<?php
/**
 * @file
 * supr_geo_content.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function supr_geo_content_default_rules_configuration() {
  $items = array();
  $items['rules_country_province_location_tagger'] = entity_import('rules_config', '{ "rules_country_province_location_tagger" : {
      "LABEL" : "Country Province Location Tagger",
      "PLUGIN" : "reaction rule",
      "WEIGHT" : "1",
      "OWNER" : "rules",
      "TAGS" : [ "Geo" ],
      "REQUIRES" : [ "rules", "rules_conditional", "wsclient" ],
      "ON" : { "node_insert" : [], "node_update" : [] },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_geofield" } },
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_locations" } },
        { "node_is_published" : { "node" : [ "node" ] } },
        { "user_has_role" : {
            "account" : [ "site:current-user" ],
            "roles" : { "value" : { "2" : "2" } }
          }
        }
      ],
      "DO" : [
        { "CONDITIONAL" : [
            {
              "IF" : { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "listing" : "listing" } } } },
              "DO" : [
                { "variable_add" : {
                    "USING" : { "type" : "decimal", "value" : [ "node:field-geofield:lat" ] },
                    "PROVIDE" : { "variable_added" : { "latitude" : "Latitude" } }
                  }
                },
                { "variable_add" : {
                    "USING" : { "type" : "decimal", "value" : [ "node:field-geofield:lon" ] },
                    "PROVIDE" : { "variable_added" : { "longitude" : "Longitude" } }
                  }
                },
                { "wsclient_geonames_country_subdivision_get_country_subdivision" : {
                    "USING" : {
                      "param_lat" : [ "latitude" ],
                      "param_lng" : [ "longitude" ],
                      "param_lang" : "en",
                      "param_username" : "demo",
                      "param_maxRows" : "3",
                      "param_radius" : "200"
                    },
                    "PROVIDE" : { "result" : { "country_subdivision_result" : "Country Subdivision Result" } }
                  }
                },
                { "variable_add" : {
                    "USING" : { "type" : "list\\u003Ctext\\u003E", "value" : [ "" ] },
                    "PROVIDE" : { "variable_added" : { "country_province_list" : "Country Province List" } }
                  }
                },
                { "data_create" : {
                    "USING" : {
                      "type" : "wsclient_geonames_country_subdivision_result_data_country_subdivision",
                      "param_distance" : [ "country-subdivision-result:distance" ],
                      "param_adminCode1" : [ "country-subdivision-result:adminCode1" ],
                      "param_countryName" : [ "country-subdivision-result:countryName" ],
                      "param_countryCode" : [ "country-subdivision-result:countryCode" ],
                      "param_codes" : [ "country-subdivision-result:codes" ],
                      "param_adminName1" : [ "country-subdivision-result:adminName1" ]
                    },
                    "PROVIDE" : { "data_created" : { "data_created" : "Created data" } }
                  }
                },
                { "list_add" : {
                    "list" : [ "country-province-list" ],
                    "item" : [ "country-subdivision-result:countryName" ],
                    "unique" : 1
                  }
                },
                { "list_add" : {
                    "list" : [ "country-province-list" ],
                    "item" : [ "country-subdivision-result:adminName1" ],
                    "unique" : 1
                  }
                },
                { "variable_add" : {
                    "USING" : {
                      "type" : "text",
                      "value" : [ "node:field-address:administrative-area" ]
                    },
                    "PROVIDE" : { "variable_added" : { "state_province" : "State \\/ Province" } }
                  }
                },
                { "variable_add" : {
                    "USING" : { "type" : "taxonomy_vocabulary", "value" : "locations" },
                    "PROVIDE" : { "variable_added" : { "locations_var" : "Locations Variable" } }
                  }
                },
                { "CONDITIONAL" : [
                    {
                      "IF" : { "NOT data_is_empty" : { "data" : [ "node:field-address:country" ] } },
                      "DO" : [
                        { "LOOP" : {
                            "USING" : { "list" : [ "node:field-locations" ] },
                            "ITEM" : { "current_locations_list_item" : "Current list item" },
                            "DO" : []
                          }
                        },
                        { "LOOP" : {
                            "USING" : { "list" : [ "country-province-list" ] },
                            "ITEM" : { "list_item" : "Current list item" },
                            "DO" : [
                              { "variable_add" : {
                                  "USING" : { "type" : "boolean", "value" : 0 },
                                  "PROVIDE" : { "variable_added" : { "term_exists_in_locations_true_variable" : "Term exists in Locations" } }
                                }
                              },
                              { "entity_query" : {
                                  "USING" : {
                                    "type" : "taxonomy_term",
                                    "property" : "name",
                                    "value" : [ "list-item" ]
                                  },
                                  "PROVIDE" : { "entity_fetched" : { "entity_fetched" : "Fetched entity" } }
                                }
                              },
                              { "LOOP" : {
                                  "USING" : { "list" : [ "entity-fetched" ] },
                                  "ITEM" : { "ef_list_item" : "Current list item" },
                                  "DO" : [
                                    { "CONDITIONAL" : [
                                        {
                                          "IF" : { "data_is" : {
                                              "data" : [ "ef-list-item:vocabulary:machine-name" ],
                                              "value" : "locations"
                                            }
                                          },
                                          "DO" : [
                                            { "data_set" : { "data" : [ "term-exists-in-locations-true-variable" ], "value" : 1 } },
                                            { "list_add" : {
                                                "list" : [ "node:field-locations" ],
                                                "item" : [ "ef-list-item" ],
                                                "unique" : 1
                                              }
                                            }
                                          ]
                                        }
                                      ]
                                    }
                                  ]
                                }
                              },
                              { "CONDITIONAL" : [
                                  {
                                    "IF" : { "data_is" : { "data" : [ "term-exists-in-locations-true-variable" ], "value" : 0 } },
                                    "DO" : [
                                      { "entity_create" : {
                                          "USING" : {
                                            "type" : "taxonomy_term",
                                            "param_name" : [ "list-item" ],
                                            "param_vocabulary" : [ "locations-var" ]
                                          },
                                          "PROVIDE" : { "entity_created" : { "term_entity_created" : "Created entity" } }
                                        }
                                      },
                                      { "entity_save" : { "data" : [ "term-entity-created" ], "immediate" : 1 } },
                                      { "list_add" : {
                                          "list" : [ "node:field-locations" ],
                                          "item" : [ "term-entity-created" ],
                                          "unique" : 1
                                        }
                                      }
                                    ]
                                  }
                                ]
                              }
                            ]
                          }
                        }
                      ]
                    }
                  ]
                }
              ]
            }
          ]
        }
      ]
    }
  }');
  $items['rules_nearby_cities_location_tagger'] = entity_import('rules_config', '{ "rules_nearby_cities_location_tagger" : {
      "LABEL" : "Nearby Cities Location Tagger",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Geo" ],
      "REQUIRES" : [ "rules", "rules_conditional", "wsclient", "devel" ],
      "ON" : { "node_insert" : [], "node_update" : [] },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_geofield" } },
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_locations" } },
        { "node_is_published" : { "node" : [ "node" ] } },
        { "user_has_role" : {
            "account" : [ "site:current-user" ],
            "roles" : { "value" : { "2" : "2" } }
          }
        }
      ],
      "DO" : [
        { "CONDITIONAL" : [
            {
              "IF" : { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "listing" : "listing" } } } },
              "DO" : [
                { "variable_add" : {
                    "USING" : { "type" : "integer", "value" : "1" },
                    "PROVIDE" : { "variable_added" : { "counter" : "Counter" } }
                  }
                },
                { "variable_add" : {
                    "USING" : { "type" : "text", "value" : "node:field-address:country-name" },
                    "PROVIDE" : { "variable_added" : { "country" : "Country" } }
                  }
                },
                { "variable_add" : {
                    "USING" : { "type" : "text", "value" : "node:field-address:state-name" },
                    "PROVIDE" : { "variable_added" : { "state_province" : "State \\/ Province" } }
                  }
                },
                { "variable_add" : {
                    "USING" : { "type" : "decimal", "value" : [ "node:field-geofield:lat" ] },
                    "PROVIDE" : { "variable_added" : { "latitude" : "Latitude" } }
                  }
                },
                { "variable_add" : {
                    "USING" : { "type" : "decimal", "value" : [ "node:field-geofield:lon" ] },
                    "PROVIDE" : { "variable_added" : { "longitude" : "Longitude" } }
                  }
                },
                { "variable_add" : {
                    "USING" : {
                      "type" : "decimal",
                      "value" : {
                        "select" : "node:field-geofield:left",
                        "num_offset" : { "value" : "-2" }
                      }
                    },
                    "PROVIDE" : { "variable_added" : { "left_latitude_neg_two_deg" : "Left Latitude Negative Two Degrees" } }
                  }
                },
                { "variable_add" : {
                    "USING" : {
                      "type" : "decimal",
                      "value" : { "select" : "node:field-geofield:top", "num_offset" : { "value" : "2" } }
                    },
                    "PROVIDE" : { "variable_added" : { "top_longitude_add_two_deg" : "Top Longitude Add Two Degrees" } }
                  }
                },
                { "variable_add" : {
                    "USING" : {
                      "type" : "decimal",
                      "value" : {
                        "select" : "node:field-geofield:right",
                        "num_offset" : { "value" : "2" }
                      }
                    },
                    "PROVIDE" : { "variable_added" : { "right_latitude_add_two_deg" : "Right Latitude Add Two Degrees" } }
                  }
                },
                { "variable_add" : {
                    "USING" : {
                      "type" : "decimal",
                      "value" : {
                        "select" : "node:field-geofield:bottom",
                        "num_offset" : { "value" : "-2" }
                      }
                    },
                    "PROVIDE" : { "variable_added" : { "bottom_longitude_neg_two_deg" : "Bottom Longitude Negative Two Degrees" } }
                  }
                },
                { "wsclient_geonames_nearby_cities_get_nearby_cities" : {
                    "USING" : {
                      "param_north" : [ "top-longitude-add-two-deg" ],
                      "param_south" : [ "bottom-longitude-neg-two-deg" ],
                      "param_east" : [ "right-latitude-add-two-deg" ],
                      "param_west" : [ "left-latitude-neg-two-deg" ],
                      "param_lang" : "en",
                      "param_username" : "demo",
                      "param_maxRows" : "3"
                    },
                    "PROVIDE" : { "result" : { "list_nearby_cities_result" : "Nearby Cities" } }
                  }
                },
                { "variable_add" : {
                    "USING" : { "type" : "taxonomy_vocabulary", "value" : "locations" },
                    "PROVIDE" : { "variable_added" : { "locations_var" : "Locations Variable" } }
                  }
                },
                { "CONDITIONAL" : [
                    {
                      "IF" : { "NOT data_is_empty" : { "data" : [ "list-nearby-cities-result:geonames" ] } },
                      "DO" : [
                        { "LOOP" : {
                            "USING" : { "list" : [ "node:field-locations" ] },
                            "ITEM" : { "current_locations_list_item" : "Current list item" },
                            "DO" : [
                              { "list_remove" : {
                                  "list" : [ "node:field-locations" ],
                                  "item" : [ "current-locations-list-item" ]
                                }
                              }
                            ]
                          }
                        },
                        { "LOOP" : {
                            "USING" : { "list" : [ "list-nearby-cities-result:geonames" ] },
                            "ITEM" : { "list_item" : "Current list item" },
                            "DO" : [
                              { "variable_add" : {
                                  "USING" : { "type" : "boolean", "value" : 0 },
                                  "PROVIDE" : { "variable_added" : { "term_exists_in_locations_true_variable" : "Term exists in Locations" } }
                                }
                              },
                              { "entity_query" : {
                                  "USING" : {
                                    "type" : "taxonomy_term",
                                    "property" : "field_geonameid",
                                    "value" : [ "list-item:geonameId" ],
                                    "limit" : "1"
                                  },
                                  "PROVIDE" : { "entity_fetched" : { "entity_fetched" : "Fetched entity" } }
                                }
                              },
                              { "CONDITIONAL" : [
                                  {
                                    "IF" : { "NOT data_is_empty" : { "data" : [ "entity-fetched" ] } },
                                    "DO" : [
                                      { "data_set" : { "data" : [ "term-exists-in-locations-true-variable" ], "value" : 1 } },
                                      { "list_add" : {
                                          "list" : [ "node:field-locations" ],
                                          "item" : [ "entity-fetched:0" ],
                                          "unique" : 1
                                        }
                                      },
                                      { "devel_debug" : { "value" : [ "entity-fetched:0:name" ] } }
                                    ]
                                  }
                                ]
                              },
                              { "CONDITIONAL" : [
                                  {
                                    "IF" : { "NOT data_is" : { "data" : [ "term-exists-in-locations-true-variable" ], "value" : 1 } },
                                    "DO" : [
                                      { "entity_create" : {
                                          "USING" : {
                                            "type" : "taxonomy_term",
                                            "param_name" : [ "list-item:name" ],
                                            "param_vocabulary" : [ "locations-var" ]
                                          },
                                          "PROVIDE" : { "entity_created" : { "term_entity_created" : "Created entity" } }
                                        }
                                      },
                                      { "entity_save" : { "data" : [ "term-entity-created" ], "immediate" : 1 } },
                                      { "entity_query" : {
                                          "USING" : {
                                            "type" : "taxonomy_term",
                                            "property" : "tid",
                                            "value" : [ "term-entity-created:tid" ],
                                            "limit" : "1"
                                          },
                                          "PROVIDE" : { "entity_fetched" : { "new_term_entity" : "New Term Entity" } }
                                        }
                                      },
                                      { "CONDITIONAL" : [
                                          {
                                            "IF" : { "entity_has_field" : { "entity" : [ "new-term-entity:0" ], "field" : "field_geonameid" } },
                                            "DO" : [
                                              { "data_set" : {
                                                  "data" : [ "new-term-entity:0:field-geonameid" ],
                                                  "value" : [ "list-item:geonameId" ]
                                                }
                                              }
                                            ]
                                          }
                                        ]
                                      },
                                      { "CONDITIONAL" : [
                                          {
                                            "IF" : { "entity_has_field" : { "entity" : [ "new-term-entity:0" ], "field" : "field_lat_lon" } },
                                            "DO" : [
                                              { "variable_add" : {
                                                  "USING" : { "type" : "text", "value" : [ "list-item:lat" ] },
                                                  "PROVIDE" : { "variable_added" : { "list_lat" : "List Lat" } }
                                                }
                                              },
                                              { "variable_add" : {
                                                  "USING" : { "type" : "text", "value" : [ "list-item:lng" ] },
                                                  "PROVIDE" : { "variable_added" : { "list_lon" : "List Lon" } }
                                                }
                                              },
                                              { "variable_add" : {
                                                  "USING" : { "type" : "text", "value" : "[list-lat:value], [list-lon:value]" },
                                                  "PROVIDE" : { "variable_added" : { "lat_lon" : "Lat Lon" } }
                                                }
                                              },
                                              { "data_set" : {
                                                  "data" : [ "new-term-entity:0:field-lat-lon" ],
                                                  "value" : [ "lat-lon" ]
                                                }
                                              }
                                            ]
                                          }
                                        ]
                                      },
                                      { "devel_debug" : { "value" : [ "list-item:name" ] } },
                                      { "list_add" : {
                                          "list" : [ "node:field-locations" ],
                                          "item" : [ "term-entity-created" ],
                                          "unique" : 1
                                        }
                                      }
                                    ]
                                  }
                                ]
                              }
                            ]
                          }
                        }
                      ]
                    }
                  ]
                },
                { "devel_debug" : { "value" : [ "node:title" ] } }
              ]
            }
          ]
        }
      ]
    }
  }');
  return $items;
}
