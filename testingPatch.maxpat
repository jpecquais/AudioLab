{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 8,
			"minor" : 3,
			"revision" : 2,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"classnamespace" : "box",
		"rect" : [ 34.0, 77.0, 1468.0, 705.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 1,
		"objectsnaponopen" : 1,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"lefttoolbarpinned" : 0,
		"toptoolbarpinned" : 0,
		"righttoolbarpinned" : 0,
		"bottomtoolbarpinned" : 0,
		"toolbars_unpinned_last_save" : 0,
		"tallnewobj" : 0,
		"boxanimatetime" : 200,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"style" : "",
		"subpatcher_template" : "",
		"assistshowspatchername" : 0,
		"boxes" : [ 			{
				"box" : 				{
					"basictuning" : 440,
					"clipheight" : 100.157894968986511,
					"data" : 					{
						"clips" : [ 							{
								"absolutepath" : "C:/Users/pecqu/Downloads/12-GTR M201-220730_1031.wav",
								"filename" : "12-GTR M201-220730_1031.wav",
								"filekind" : "audiofile",
								"id" : "u298000339",
								"selection" : [ 0.046948356807512, 0.133802816901408 ],
								"loop" : 1,
								"content_state" : 								{
									"loop" : 1
								}

							}
 ]
					}
,
					"followglobaltempo" : 0,
					"formantcorrection" : 0,
					"id" : "obj-13",
					"maxclass" : "playlist~",
					"mode" : "basic",
					"numinlets" : 1,
					"numoutlets" : 5,
					"originallength" : [ 0.0, "ticks" ],
					"originaltempo" : 120.0,
					"outlettype" : [ "signal", "signal", "signal", "", "dictionary" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 678.0, 44.184210062026978, 481.0, 101.157894968986511 ],
					"pitchcorrection" : 0,
					"quality" : "basic",
					"timestretch" : [ 0 ]
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-6",
					"maxclass" : "number~",
					"mode" : 2,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "float" ],
					"patching_rect" : [ 399.0, 406.0, 80.0, 22.0 ],
					"sig" : 0.0
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-9",
					"maxclass" : "live.meter~",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "float", "int" ],
					"patching_rect" : [ 414.45999999999998, 175.492540836334229, 5.0, 100.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-2",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 407.45999999999998, 110.450000000000003, 52.0, 22.0 ],
					"text" : "adc~ 10"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-15",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 652.605258464813232, 469.551067113876343, 31.0, 22.0 ],
					"text" : "plug"
				}

			}
, 			{
				"box" : 				{
					"autosave" : 1,
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"id" : "obj-18",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 8,
					"offset" : [ 0.0, 0.0 ],
					"outlettype" : [ "signal", "signal", "", "list", "int", "", "", "" ],
					"patching_rect" : [ 652.605258464813232, 527.551067113876343, 92.5, 22.0 ],
					"save" : [ "#N", "vst~", "loaduniqueid", 0, ";" ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_invisible" : 1,
							"parameter_longname" : "vst~[1]",
							"parameter_shortname" : "vst~",
							"parameter_type" : 3
						}

					}
,
					"saved_object_attributes" : 					{
						"parameter_enable" : 1,
						"parameter_mappable" : 0
					}
,
					"text" : "vst~",
					"varname" : "vst~[1]",
					"viewvisibility" : 0
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-12",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "signal" ],
					"patching_rect" : [ 652.605258464813232, 439.342105031013489, 84.0, 22.0 ],
					"text" : "mc.unpack~ 2"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-17",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 693.605258464813232, 289.342105031013489, 31.0, 22.0 ],
					"text" : "plug"
				}

			}
, 			{
				"box" : 				{
					"autosave" : 1,
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"id" : "obj-16",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 8,
					"offset" : [ 0.0, 0.0 ],
					"outlettype" : [ "signal", "signal", "", "list", "int", "", "", "" ],
					"patching_rect" : [ 693.605258464813232, 329.342105031013489, 92.5, 22.0 ],
					"save" : [ "#N", "vst~", "loaduniqueid", 0, ";" ],
					"saved_attribute_attributes" : 					{
						"valueof" : 						{
							"parameter_invisible" : 1,
							"parameter_longname" : "vst~",
							"parameter_shortname" : "vst~",
							"parameter_type" : 3
						}

					}
,
					"saved_object_attributes" : 					{
						"parameter_enable" : 1,
						"parameter_mappable" : 0
					}
,
					"snapshot" : 					{
						"filetype" : "C74Snapshot",
						"version" : 2,
						"minorversion" : 0,
						"name" : "snapshotlist",
						"origin" : "vst~",
						"type" : "list",
						"subtype" : "Undefined",
						"embed" : 1,
						"snapshot" : 						{
							"pluginname" : "Bertom_EQCurveAnalyzer.vst3",
							"plugindisplayname" : "EQ Curve Analyzer",
							"pluginsavedname" : "C:/Program Files/Common Files/VST3/Bertom_EQCurveAnalyzer.vst3",
							"pluginsaveduniqueid" : 0,
							"version" : 1,
							"isbank" : 0,
							"isbase64" : 1,
							"blob" : "819.VMjLgnx....OVMEUy.Ea0cVZtMEcgQWY9vSRC8Vav8lak4Fc9TSM43BTEwlXmAiUYoWUrIVdA4hKAomKTETRUAUSAAkKBolQY4BQlETQDQUUTgCQtDSQFEFLUYjKAolKA4hKt3hKt3hK1sxPt.UQpQUPvPjKAgDTZoVPP4BRTYFTYETUPMUSE4RLEYTXvTkQtDjZtDjKt3hKt3hKt3hKt3BTEoFUAACQtDDRPokZAAkKJQkYQYTTvLkTQQUTREjYic1cVM1ZAAkKIAkKt3hKt3hKtXlRtDjKTETRUAUSAAkKBolQY4BQtHTQhQUTNgCUUQUPlM1Y2Y0XqEDTtjDTt3hKt3hKt3hKJ4xPtPUPIUETMEDTtHjZFkkKDYWPEIlZT8TUEQkKXcEVxU0UY4BQPIDQt3hKt3hKt3hKq7jKtTETRUDUS4BQl4xaQYjKAoFTAwTQDUUQznGTYEjYic1cVM1ZAAkKIAkKt3hKt3hKt3hKt3hKTETRUAUSAAkKBolQY4BQPITQ1o2TGkkZTUTQE4RLEYTXvTkQtDjZtDjKt3hKt3hKtX2JC4BTEoFUAACQtDDRPokZAAkKGQETSEzYEEkPAY1XmcmUisVPP4RRP4hKt3hKt3hKtnjKA4BUAkTUP0TPP4hPpYTVtPDTBUjdTAEVYoFUEUTQtDSQFEFLUYjKAolKA4hKt3hKt3BZSMDQt.UQpQUPvPjKAgDTZoVPP4RRTA0TIQiZQIUUTQkKXcEVxU0UY4BQPIDQt3hKt3hKt3BTCAkKtTETRUDUS4BQl4xaQYjKAYFTAMUUDU0QUo1Ttf0UXIWUWkkKDAkPD4hKt3hKt3hKtrxSt3RUPIUQTMkKDYlKuEkQtDjbPEzTmo2TWETQRETSUEkKXcEVxU0UY4BQPIDQt3hKt3hKt3hKt3hKtTETRUDUS4BQl4xaQYjKA4FTAUEMDEUQIUzTAsVQtDSQFEFLUYjKAolKA4hKt3hKt3hKt3hKt3hKt3hKt3hKt3hKt3hKt3hKt3hKt3FUUMTUDQEdqw1XmE0UYQTQFM1YAwyKIMzasA2atUlaz4COuX0TTMCTrU2Yo41TzEFck4C."
						}
,
						"snapshotlist" : 						{
							"current_snapshot" : 0,
							"entries" : [ 								{
									"filetype" : "C74Snapshot",
									"version" : 2,
									"minorversion" : 0,
									"name" : "EQ Curve Analyzer",
									"origin" : "Bertom_EQCurveAnalyzer.vst3",
									"type" : "VST3",
									"subtype" : "AudioEffect",
									"embed" : 1,
									"snapshot" : 									{
										"pluginname" : "Bertom_EQCurveAnalyzer.vst3",
										"plugindisplayname" : "EQ Curve Analyzer",
										"pluginsavedname" : "C:/Program Files/Common Files/VST3/Bertom_EQCurveAnalyzer.vst3",
										"pluginsaveduniqueid" : 0,
										"version" : 1,
										"isbank" : 0,
										"isbase64" : 1,
										"blob" : "819.VMjLgnx....OVMEUy.Ea0cVZtMEcgQWY9vSRC8Vav8lak4Fc9TSM43BTEwlXmAiUYoWUrIVdA4hKAomKTETRUAUSAAkKBolQY4BQlETQDQUUTgCQtDSQFEFLUYjKAolKA4hKt3hKt3hK1sxPt.UQpQUPvPjKAgDTZoVPP4BRTYFTYETUPMUSE4RLEYTXvTkQtDjZtDjKt3hKt3hKt3hKt3BTEoFUAACQtDDRPokZAAkKJQkYQYTTvLkTQQUTREjYic1cVM1ZAAkKIAkKt3hKt3hKtXlRtDjKTETRUAUSAAkKBolQY4BQtHTQhQUTNgCUUQUPlM1Y2Y0XqEDTtjDTt3hKt3hKt3hKJ4xPtPUPIUETMEDTtHjZFkkKDYWPEIlZT8TUEQkKXcEVxU0UY4BQPIDQt3hKt3hKt3hKq7jKtTETRUDUS4BQl4xaQYjKAoFTAwTQDUUQznGTYEjYic1cVM1ZAAkKIAkKt3hKt3hKt3hKt3hKTETRUAUSAAkKBolQY4BQPITQ1o2TGkkZTUTQE4RLEYTXvTkQtDjZtDjKt3hKt3hKtX2JC4BTEoFUAACQtDDRPokZAAkKGQETSEzYEEkPAY1XmcmUisVPP4RRP4hKt3hKt3hKtnjKA4BUAkTUP0TPP4hPpYTVtPDTBUjdTAEVYoFUEUTQtDSQFEFLUYjKAolKA4hKt3hKt3BZSMDQt.UQpQUPvPjKAgDTZoVPP4RRTA0TIQiZQIUUTQkKXcEVxU0UY4BQPIDQt3hKt3hKt3BTCAkKtTETRUDUS4BQl4xaQYjKAYFTAMUUDU0QUo1Ttf0UXIWUWkkKDAkPD4hKt3hKt3hKtrxSt3RUPIUQTMkKDYlKuEkQtDjbPEzTmo2TWETQRETSUEkKXcEVxU0UY4BQPIDQt3hKt3hKt3hKt3hKtTETRUDUS4BQl4xaQYjKA4FTAUEMDEUQIUzTAsVQtDSQFEFLUYjKAolKA4hKt3hKt3hKt3hKt3hKt3hKt3hKt3hKt3hKt3hKt3hKt3FUUMTUDQEdqw1XmE0UYQTQFM1YAwyKIMzasA2atUlaz4COuX0TTMCTrU2Yo41TzEFck4C."
									}
,
									"fileref" : 									{
										"name" : "EQ Curve Analyzer",
										"filename" : "EQ Curve Analyzer.maxsnap",
										"filepath" : "~/Documents/Max 8/Snapshots",
										"filepos" : -1,
										"snapshotfileid" : "16e45bcf5973470aa6ad1867bde5f73d"
									}

								}
 ]
						}

					}
,
					"text" : "vst~",
					"varname" : "vst~",
					"viewvisibility" : 0
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-11",
					"maxclass" : "mc.ezdac~",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 492.105258464813232, 605.342105031013489, 45.0, 45.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-8",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "multichannelsignal" ],
					"patching_rect" : [ 495.605258464813232, 293.342105031013489, 80.0, 22.0 ],
					"text" : "mc.combine~"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-7",
					"maxclass" : "gain~",
					"multichannelvariant" : 1,
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "multichannelsignal", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 495.605258464813232, 363.342105031013489, 22.0, 140.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-5",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 495.605258464813232, 70.342105031013489, 38.0, 22.0 ],
					"text" : "pink~"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-4",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 495.605258464813232, 128.342105031013489, 58.0, 22.0 ],
					"text" : "loadbang"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-3",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 495.605258464813232, 161.342105031013489, 127.0, 22.0 ],
					"text" : "read noteDetector.dsp"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-1",
					"library_path0" : "",
					"library_path1" : "C:\\Users\\pecqu\\Documents\\Max 8\\Packages\\faustgen\\externals\\msp\\faustgen-resources\\",
					"library_path2" : "C:\\Users\\pecqu\\OneDrive\\Documents\\Max 8\\Packages\\faustgen\\externals\\msp\\faustgen-resources\\",
					"machinecode" : "ZIYGAAAAAAApCQAAHQAAAAAAAAAudGV4dAAAAAAAAAAAAAAAwQEAAAQBAADFAgAAAAAAAAMAAAAgAFBgLmRhdGEAAAAAAAAAAAAAAAAAAADjAgAAAAAAAAAAAAAAAAAAQAAwwC5ic3MAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIAAMMAucmRhdGEAAAAAAAAAAAAAEAAAAOMCAAAAAAAAAAAAAAAAAABAEFBALnJkYXRhAAAAAAAAAAAAAAgAAADzAgAAAAAAAAAAAAAAAAAAQBBAQC5yZGF0YQAAAAAAAAAAAAAuBgAA+wIAAAAAAAAAAAAAAAAAAEAAUEDDZi4PH4QAAAAAAA8fRAAAxfhXwMX4EQHDDx+AAAAAAIlREMNmLg8fhAAAAAAAZpDDZi4PH4QAAAAAAA8fRAAAw2YuDx+EAAAAAAAPH0QAAEi4AAAAAAAAAADDDx9EAABWV4XSD45UAQAATYsASYsBidLF+xBBCEiNcv9BidNBg+MDSbkAAAAAAAAAAEm6AAAAAAAAAABIg/4Dcwcx9unMAAAATCnaMfbEwXkoCcTBexASZi4PH4QAAAAAAA8fQADEwXsQHPDF4VTZxfkTGcXjXMDF+1nCxfsRBPBIizlIiXkIxOH5bsfEwXsQXPAIxeFU2cX5ExnF41zAxftZwsX7EUTwCEiLOUiJeQjE4flux8TBexBc8BDF4VTZxfkTGcXjXMDF+1nCxfsRRPAQSIs5SIl5CMTh+W7HxMF7EFzwGMXhVNnF+RMZxeNcwMX7WcLF+xFE8BhIizlIiXkISIPGBMTh+W7HSDnyD4VR////TYXbdEpIjQTwSY0U8DH2xMF5KAnEwXsQEg8fhAAAAAAAxfsQHPLF4VTZxfkTGcXjXMDF+1nCxfsRBPBIizlIiXkIxOH5bsdI/8ZJOfN10l9ew1IAAAAVAAAAAQCEAAAACAAAAAEAjgAAAAsAAAABAP////////9//////////38AAAAAAAAkQHsibmFtZSI6ICJOb3RlIERldGVjdG9yIiwiZmlsZW5hbWUiOiAiZmF1c3RnZW4tMSIsInZlcnNpb24iOiAiMi40MS4xIiwiY29tcGlsZV9vcHRpb25zIjogIi1sYW5nIGxsdm0gMTAuMC4wIC1lcyAxIC1tY2QgMTYgLWRvdWJsZSAtZnR6IDAiLCJsaWJyYXJ5X2xpc3QiOiBbIkM6XFVzZXJzXHBlY3F1XE9uZURyaXZlXERvY3VtZW50c1xNYXggOFxQYWNrYWdlc1xmYXVzdGdlblxleHRlcm5hbHNcbXNwXGZhdXN0Z2VuLXJlc291cmNlcy9zdGRmYXVzdC5saWIiLCJDOlxVc2Vyc1xwZWNxdVxPbmVEcml2ZVxEb2N1bWVudHNcTWF4IDhcUGFja2FnZXNcZmF1c3RnZW5cZXh0ZXJuYWxzXG1zcFxmYXVzdGdlbi1yZXNvdXJjZXMvZmlsdGVycy5saWIiXSwiaW5jbHVkZV9wYXRobmFtZXMiOiBbIkM6XFVzZXJzXHBlY3F1XE9uZURyaXZlXERvY3VtZW50c1xNYXggOFxQYWNrYWdlc1xmYXVzdGdlblxleHRlcm5hbHNcbXNwXGZhdXN0Z2VuLXJlc291cmNlc1wiLCJDOlxVc2Vyc1xwZWNxdVxEb2N1bWVudHNcTWF4IDhcUGFja2FnZXNcZmF1c3RnZW5cZXh0ZXJuYWxzXG1zcFxmYXVzdGdlbi1yZXNvdXJjZXNcIiwiQzpcUHJvZ3JhbSBGaWxlc1xDeWNsaW5nICc3NC9zaGFyZS9mYXVzdCIsIi91c3IvbG9jYWwvc2hhcmUvZmF1c3QiLCIvdXNyL3NoYXJlL2ZhdXN0IiwiLiJdLCJzaXplIjogMjAsImlucHV0cyI6IDEsIm91dHB1dHMiOiAxLCJzcl9pbmRleCI6IDE2LCJtZXRhIjogWyB7ICJhdXRob3IiOiAiSmVhbi1Mb3VwIFBlY3F1YWlzIiB9LHsgImNvbXBpbGVfb3B0aW9ucyI6ICItbGFuZyBsbHZtIDEwLjAuMCAtZXMgMSAtbWNkIDE2IC1kb3VibGUgLWZ0eiAwIiB9LHsgImZpbGVuYW1lIjogImZhdXN0Z2VuLTEiIH0seyAiZmlsdGVycy5saWIvY29udjphdXRob3IiOiAiSnVsaXVzIE8uIFNtaXRoIElJSSIgfSx7ICJmaWx0ZXJzLmxpYi9jb252OmNvcHlyaWdodCI6ICJDb3B5cmlnaHQgKEMpIDIwMDMtMjAxOSBieSBKdWxpdXMgTy4gU21pdGggSUlJIDxqb3NfY2NybWEuc3RhbmZvcmQuZWR1PiIgfSx7ICJmaWx0ZXJzLmxpYi9jb252OmxpY2Vuc2UiOiAiTUlULXN0eWxlIFNUSy00LjMgbGljZW5zZSIgfSx7ICJmaWx0ZXJzLmxpYi9maXI6YXV0aG9yIjogIkp1bGl1cyBPLiBTbWl0aCBJSUkiIH0seyAiZmlsdGVycy5saWIvZmlyOmNvcHlyaWdodCI6ICJDb3B5cmlnaHQgKEMpIDIwMDMtMjAxOSBieSBKdWxpdXMgTy4gU21pdGggSUlJIDxqb3NfY2NybWEuc3RhbmZvcmQuZWR1PiIgfSx7ICJmaWx0ZXJzLmxpYi9maXI6bGljZW5zZSI6ICJNSVQtc3R5bGUgU1RLLTQuMyBsaWNlbnNlIiB9LHsgImZpbHRlcnMubGliL2xvd3Bhc3MwX2hpZ2hwYXNzMSI6ICJNSVQtc3R5bGUgU1RLLTQuMyBsaWNlbnNlIiB9LHsgImZpbHRlcnMubGliL25hbWUiOiAiRmF1c3QgRmlsdGVycyBMaWJyYXJ5IiB9LHsgImZpbHRlcnMubGliL3ZlcnNpb24iOiAiMC4zIiB9LHsgImxpY2Vuc2UiOiAiR1BMMyIgfSx7ICJuYW1lIjogIk5vdGUgRGV0ZWN0b3IiIH0seyAidmVyc2lvbiI6ICIxLjAwIiB9XSwidWkiOiBbIHsidHlwZSI6ICJ2Z3JvdXAiLCJsYWJlbCI6ICJOb3RlIERldGVjdG9yIiwiaXRlbXMiOiBbXX1dfQAudGV4dAAAAAAAAAABAAAAAwHBAQAAAwAAAAsH/boBAAAAAAAuZGF0YQAAAAAAAAACAAAAAwEAAAAAAAAAAAAAAAACAAAAAAAuYnNzAAAAAAAAAAADAAAAAwEAAAAAAAAAAAAAAAADAAAAAAAucmRhdGEAAAAAAAAEAAAAAwEQAAAAAAAAACAO5VgEAAIAAAAAAAAAoAYAAAAAAAAEAAAAAgAucmRhdGEAAAAAAAAFAAAAAwEIAAAAAAAAADagNIcFAAIAAAAAAAAAxwYAAAAAAAAFAAAAAgAucmRhdGEAAAAAAAAGAAAAAwEuBgAAAAAAAO/aY+8GAAAAAABAZmVhdC4wMAAAAAD//wAAAwAAAAAAPwYAAAAAAAABACAAAgAAAAAAZQYAABAAAAABACAAAgAAAAAATgYAACAAAAABACAAAgAAAAAAhQYAADAAAAABACAAAgAAAAAAMgYAAEAAAAABACAAAgAAAAAAkwYAAFAAAAABACAAAgAAAAAABAAAAAAAAAAGAAAAAwAAAAAAeAYAAGAAAAABACAAAgBfZmx0dXNlZAAAAAAAAAAAAgAuZmlsZQAAAAAAAAD+/wAAZwQtbGFuZyBsbHZtIDEwLjAuMCAtZXMgMSAtbWNkIDE2IC1kb3VibGUgLWZ0eiAwLCB2Mi40MS4xAAAAAAAAAAAAAAAAAAAAAADfBgAAeyJuYW1lIjogIk5vdGUgRGV0ZWN0b3IiLCJmaWxlbmFtZSI6ICJmYXVzdGdlbi0xIiwidmVyc2lvbiI6ICIyLjQxLjEiLCJjb21waWxlX29wdGlvbnMiOiAiLWxhbmcgbGx2bSAxMC4wLjAgLWVzIDEgLW1jZCAxNiAtZG91YmxlIC1mdHogMCIsImxpYnJhcnlfbGlzdCI6IFsiQzpcVXNlcnNccGVjcXVcT25lRHJpdmVcRG9jdW1lbnRzXE1heCA4XFBhY2thZ2VzXGZhdXN0Z2VuXGV4dGVybmFsc1xtc3BcZmF1c3RnZW4tcmVzb3VyY2VzL3N0ZGZhdXN0LmxpYiIsIkM6XFVzZXJzXHBlY3F1XE9uZURyaXZlXERvY3VtZW50c1xNYXggOFxQYWNrYWdlc1xmYXVzdGdlblxleHRlcm5hbHNcbXNwXGZhdXN0Z2VuLXJlc291cmNlcy9maWx0ZXJzLmxpYiJdLCJpbmNsdWRlX3BhdGhuYW1lcyI6IFsiQzpcVXNlcnNccGVjcXVcT25lRHJpdmVcRG9jdW1lbnRzXE1heCA4XFBhY2thZ2VzXGZhdXN0Z2VuXGV4dGVybmFsc1xtc3BcZmF1c3RnZW4tcmVzb3VyY2VzXCIsIkM6XFVzZXJzXHBlY3F1XERvY3VtZW50c1xNYXggOFxQYWNrYWdlc1xmYXVzdGdlblxleHRlcm5hbHNcbXNwXGZhdXN0Z2VuLXJlc291cmNlc1wiLCJDOlxQcm9ncmFtIEZpbGVzXEN5Y2xpbmcgJzc0L3NoYXJlL2ZhdXN0IiwiL3Vzci9sb2NhbC9zaGFyZS9mYXVzdCIsIi91c3Ivc2hhcmUvZmF1c3QiLCIuIl0sInNpemUiOiAyMCwiaW5wdXRzIjogMSwib3V0cHV0cyI6IDEsInNyX2luZGV4IjogMTYsIm1ldGEiOiBbIHsgImF1dGhvciI6ICJKZWFuLUxvdXAgUGVjcXVhaXMiIH0seyAiY29tcGlsZV9vcHRpb25zIjogIi1sYW5nIGxsdm0gMTAuMC4wIC1lcyAxIC1tY2QgMTYgLWRvdWJsZSAtZnR6IDAiIH0seyAiZmlsZW5hbWUiOiAiZmF1c3RnZW4tMSIgfSx7ICJmaWx0ZXJzLmxpYi9jb252OmF1dGhvciI6ICJKdWxpdXMgTy4gU21pdGggSUlJIiB9LHsgImZpbHRlcnMubGliL2NvbnY6Y29weXJpZ2h0IjogIkNvcHlyaWdodCAoQykgMjAwMy0yMDE5IGJ5IEp1bGl1cyBPLiBTbWl0aCBJSUkgPGpvc19jY3JtYS5zdGFuZm9yZC5lZHU+IiB9LHsgImZpbHRlcnMubGliL2NvbnY6bGljZW5zZSI6ICJNSVQtc3R5bGUgU1RLLTQuMyBsaWNlbnNlIiB9LHsgImZpbHRlcnMubGliL2ZpcjphdXRob3IiOiAiSnVsaXVzIE8uIFNtaXRoIElJSSIgfSx7ICJmaWx0ZXJzLmxpYi9maXI6Y29weXJpZ2h0IjogIkNvcHlyaWdodCAoQykgMjAwMy0yMDE5IGJ5IEp1bGl1cyBPLiBTbWl0aCBJSUkgPGpvc19jY3JtYS5zdGFuZm9yZC5lZHU+IiB9LHsgImZpbHRlcnMubGliL2ZpcjpsaWNlbnNlIjogIk1JVC1zdHlsZSBTVEstNC4zIGxpY2Vuc2UiIH0seyAiZmlsdGVycy5saWIvbG93cGFzczBfaGlnaHBhc3MxIjogIk1JVC1zdHlsZSBTVEstNC4zIGxpY2Vuc2UiIH0seyAiZmlsdGVycy5saWIvbmFtZSI6ICJGYXVzdCBGaWx0ZXJzIExpYnJhcnkiIH0seyAiZmlsdGVycy5saWIvdmVyc2lvbiI6ICIwLjMiIH0seyAibGljZW5zZSI6ICJHUEwzIiB9LHsgIm5hbWUiOiAiTm90ZSBEZXRlY3RvciIgfSx7ICJ2ZXJzaW9uIjogIjEuMDAiIH1dLCJ1aSI6IFsgeyJ0eXBlIjogInZncm91cCIsImxhYmVsIjogIk5vdGUgRGV0ZWN0b3IiLCJpdGVtcyI6IFtdfV19AGRlc3Ryb3lteWRzcABjbGFzc0luaXRteWRzcABpbnN0YW5jZUNvbnN0YW50c215ZHNwAGluc3RhbmNlQ2xlYXJteWRzcABjb21wdXRlbXlkc3AAYWxsb2NhdGVteWRzcABnZXRKU09ObXlkc3AAX194bW1AN2ZmZmZmZmZmZmZmZmZmZjdmZmZmZmZmZmZmZmZmZmYAX19yZWFsQDQwMjQwMDAwMDAwMDAwMDAA",
					"machinecode_size" : 6168,
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "signal", "", "" ],
					"patching_rect" : [ 495.605258464813232, 242.342105031013489, 202.0, 22.0 ],
					"sample_format" : 1,
					"serial_number" : "JLP-LAPTOP64 bits",
					"sourcecode" : "declare name \"Note Detector\";\ndeclare author \"Jean-Loup Pecquais\";\ndeclare version \"1.00\";\ndeclare license \"GPL3\"; \n\nimport(\"stdfaust.lib\");\n\nsigVariation = fi.conv((1,-1)); //Allows to compute de derivation of the input signal\n\nprocess = abs : sigVariation : *(10);",
					"sourcecode_size" : 267,
					"text" : "faustgen~",
					"varname" : "faustgen-550512960",
					"version" : "1.61"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-6", 0 ],
					"order" : 1,
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-8", 0 ],
					"order" : 0,
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-18", 1 ],
					"source" : [ "obj-12", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-18", 0 ],
					"source" : [ "obj-12", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"source" : [ "obj-13", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-18", 0 ],
					"source" : [ "obj-15", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"source" : [ "obj-17", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-1", 0 ],
					"source" : [ "obj-3", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-3", 0 ],
					"source" : [ "obj-4", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 0 ],
					"source" : [ "obj-7", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 0 ],
					"order" : 0,
					"source" : [ "obj-8", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-7", 0 ],
					"order" : 1,
					"source" : [ "obj-8", 0 ]
				}

			}
 ],
		"parameters" : 		{
			"obj-16" : [ "vst~", "vst~", 0 ],
			"obj-18" : [ "vst~[1]", "vst~", 0 ],
			"parameterbanks" : 			{
				"0" : 				{
					"index" : 0,
					"name" : "",
					"parameters" : [ "-", "-", "-", "-", "-", "-", "-", "-" ]
				}

			}
,
			"inherited_shortname" : 1
		}
,
		"dependency_cache" : [ 			{
				"name" : "12-GTR M201-220730_1031.wav",
				"bootpath" : "~/Downloads",
				"patcherrelativepath" : "../../../../../Downloads",
				"type" : "WAVE",
				"implicit" : 1
			}
, 			{
				"name" : "faustgen~.mxe64",
				"type" : "mx64"
			}
 ],
		"autosave" : 0
	}

}
