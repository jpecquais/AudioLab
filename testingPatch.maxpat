{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 8,
			"minor" : 3,
			"revision" : 1,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"classnamespace" : "box",
		"rect" : [ 2602.0, 85.0, 1852.0, 969.0 ],
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
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"id" : "obj-6",
					"maxclass" : "number~",
					"mode" : 2,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "float" ],
					"patching_rect" : [ 399.0, 406.0, 56.0, 22.0 ],
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
					"basictuning" : 440,
					"clipheight" : 67.0,
					"data" : 					{
						"clips" : [ 							{
								"absolutepath" : "C:/Users/pecqu/Music/12-GTR M201-220730_1031.wav",
								"filename" : "12-GTR M201-220730_1031.wav",
								"filekind" : "audiofile",
								"id" : "u531000344",
								"selection" : [ 0.095354523227384, 0.132029339853301 ],
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
					"id" : "obj-24",
					"maxclass" : "playlist~",
					"mode" : "basic",
					"numinlets" : 1,
					"numoutlets" : 5,
					"originallength" : [ 0.0, "ticks" ],
					"originaltempo" : 120.0,
					"outlettype" : [ "signal", "signal", "signal", "", "dictionary" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 554.605258464813232, 24.342105031013489, 464.0, 68.0 ],
					"pitchcorrection" : 0,
					"quality" : "basic",
					"timestretch" : [ 0 ]
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
					"patching_rect" : [ 612.605258464813232, 117.342105031013489, 58.0, 22.0 ],
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
					"patching_rect" : [ 612.605258464813232, 150.342105031013489, 132.0, 22.0 ],
					"text" : "read guitarToSynth.dsp"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-1",
					"library_path0" : "",
					"library_path1" : "C:\\Users\\pecqu\\Documents\\Max 8\\Packages\\faustgen\\externals\\msp\\faustgen-resources\\",
					"machinecode" : "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ICItbGFuZyBsbHZtIDEwLjAuMCAtZXMgMSAtbWNkIDE2IC1kb3VibGUgLWZ0eiAwIiwibGlicmFyeV9saXN0IjogWyJDOlxVc2Vyc1xwZWNxdVxEb2N1bWVudHNcTWF4IDhcUGFja2FnZXNcZmF1c3RnZW5cZXh0ZXJuYWxzXG1zcFxmYXVzdGdlbi1yZXNvdXJjZXMvc3RkZmF1c3QubGliIiwiQzpcVXNlcnNccGVjcXVcRG9jdW1lbnRzXE1heCA4XFBhY2thZ2VzXGZhdXN0Z2VuXGV4dGVybmFsc1xtc3BcZmF1c3RnZW4tcmVzb3VyY2VzL2FuYWx5emVycy5saWIiLCJDOlxVc2Vyc1xwZWNxdVxEb2N1bWVudHNcTWF4IDhcUGFja2FnZXNcZmF1c3RnZW5cZXh0ZXJuYWxzXG1zcFxmYXVzdGdlbi1yZXNvdXJjZXMvZW52ZWxvcGVzLmxpYiIsIkM6XFVzZXJzXHBlY3F1XERvY3VtZW50c1xNYXggOFxQYWNrYWdlc1xmYXVzdGdlblxleHRlcm5hbHNcbXNwXGZhdXN0Z2VuLXJlc291cmNlcy9zaWduYWxzLmxpYiIsIkM6XFVzZXJzXHBlY3F1XERvY3VtZW50c1xNYXggOFxQYWNrYWdlc1xmYXVzdGdlblxleHRlcm5hbHNcbXNwXGZhdXN0Z2VuLXJlc291cmNlcy9tYXRocy5saWIiLCJDOlxVc2Vyc1xwZWNxdVxEb2N1bWVudHNcTWF4IDhcUGFja2FnZXNcZmF1c3RnZW5cZXh0ZXJuYWxzXG1zcFxmYXVzdGdlbi1yZXNvdXJjZXMvcGxhdGZvcm0ubGliIiwiQzpcVXNlcnNccGVjcXVcRG9jdW1lbnRzXE1heCA4XFBhY2thZ2VzXGZhdXN0Z2VuXGV4dGVybmFsc1xtc3BcZmF1c3RnZW4tcmVzb3VyY2VzL2Jhc2ljcy5saWIiLCJDOlxVc2Vyc1xwZWNxdVxEb2N1bWVudHNcTWF4IDhcUGFja2FnZXNcZmF1c3RnZW5cZXh0ZXJuYWxzXG1zcFxmYXVzdGdlbi1yZXNvdXJjZXMvb3NjaWxsYXRvcnMubGliIiwiQzpcVXNlcnNccGVjcXVcRG9jdW1lbnRzXE1heCA4XFBhY2thZ2VzXGZhdXN0Z2VuXGV4dGVybmFsc1xtc3BcZmF1c3RnZW4tcmVzb3VyY2VzL2ZpbHRlcnMubGliIiwiQzpcVXNlcnNccGVjcXVcRG9jdW1lbnRzXE1heCA4XFBhY2thZ2VzXGZhdXN0Z2VuXGV4dGVybmFsc1xtc3BcZmF1c3RnZW4tcmVzb3VyY2VzL3JvdXRlcy5saWIiXSwiaW5jbHVkZV9wYXRobmFtZXMiOiBbIkM6XFVzZXJzXHBlY3F1XERvY3VtZW50c1xNYXggOFxQYWNrYWdlc1xmYXVzdGdlblxleHRlcm5hbHNcbXNwXGZhdXN0Z2VuLXJlc291cmNlc1wiLCJDOlxQcm9ncmFtIEZpbGVzXEN5Y2xpbmcgJzc0L3NoYXJlL2ZhdXN0IiwiL3Vzci9sb2NhbC9zaGFyZS9mYXVzdCIsIi91c3Ivc2hhcmUvZmF1c3QiLCIuIl0sInNpemUiOiAzMzMyMCwiaW5wdXRzIjogMSwib3V0cHV0cyI6IDEsInNyX2luZGV4IjogMCwibWV0YSI6IFsgeyAiYW5hbHl6ZXJzLmxpYi9uYW1lIjogIkZhdXN0IEFuYWx5emVyIExpYnJhcnkiIH0seyAiYW5hbHl6ZXJzLmxpYi9waXRjaFRyYWNrZXI6YXV0aG9yIjogIkRhcmlvIFNhbmZpbGlwcG8iIH0seyAiYW5hbHl6ZXJzLmxpYi9waXRjaFRyYWNrZXI6Y29weXJpZ2h0IjogIkNvcHlyaWdodCAoQykgMjAyMiBEYXJpbyBTYW5maWxpcHBvICAgICAgIDxzYW5maWxpcHBvLmRhcmlvX2dtYWlsLmNvbT4iIH0seyAiYW5hbHl6ZXJzLmxpYi9waXRjaFRyYWNrZXI6bGljZW5zZSI6ICJNSVQgTGljZW5zZSIgfSx7ICJhbmFseXplcnMubGliL3ZlcnNpb24iOiAiMC4yIiB9LHsgImFuYWx5emVycy5saWIvemNyOmF1dGhvciI6ICJEYXJpbyBTYW5maWxpcHBvIiB9LHsgImFuYWx5emVycy5saWIvemNyOmNvcHlyaWdodCI6ICJDb3B5cmlnaHQgKEMpIDIwMjAgRGFyaW8gU2FuZmlsaXBwbyAgICAgICA8c2FuZmlsaXBwby5kYXJpb19nbWFpbC5jb20+IiB9LHsgImFuYWx5emVycy5saWIvemNyOmxpY2Vuc2UiOiAiTUlULXN0eWxlIFNUSy00LjMgbGljZW5zZSIgfSx7ICJiYXNpY3MubGliL25hbWUiOiAiRmF1c3QgQmFzaWMgRWxlbWVudCBMaWJyYXJ5IiB9LHsgImJhc2ljcy5saWIvdmVyc2lvbiI6ICIwLjgiIH0seyAiY29tcGlsZV9vcHRpb25zIjogIi1sYW5nIGxsdm0gMTAuMC4wIC1lcyAxIC1tY2QgMTYgLWRvdWJsZSAtZnR6IDAiIH0seyAiZW52ZWxvcGVzLmxpYi9hZHNyOmF1dGhvciI6ICJZYW5uIE9ybGFyZXkgYW5kIEFuZHJleSBCdW5kaW4iIH0seyAiZW52ZWxvcGVzLmxpYi9hdXRob3IiOiAiR1JBTUUiIH0seyAiZW52ZWxvcGVzLmxpYi9jb3B5cmlnaHQiOiAiR1JBTUUiIH0seyAiZW52ZWxvcGVzLmxpYi9saWNlbnNlIjogIkxHUEwgd2l0aCBleGNlcHRpb24iIH0seyAiZW52ZWxvcGVzLmxpYi9uYW1lIjogIkZhdXN0IEVudmVsb3BlIExpYnJhcnkiIH0seyAiZW52ZWxvcGVzLmxpYi92ZXJzaW9uIjogIjAuMiIgfSx7ICJmaWxlbmFtZSI6ICJmYXVzdGdlbi0zIiB9LHsgImZpbHRlcnMubGliL2ZpcjphdXRob3IiOiAiSnVsaXVzIE8uIFNtaXRoIElJSSIgfSx7ICJmaWx0ZXJzLmxpYi9maXI6Y29weXJpZ2h0IjogIkNvcHlyaWdodCAoQykgMjAwMy0yMDE5IGJ5IEp1bGl1cyBPLiBTbWl0aCBJSUkgPGpvc19jY3JtYS5zdGFuZm9yZC5lZHU+IiB9LHsgImZpbHRlcnMubGliL2ZpcjpsaWNlbnNlIjogIk1JVC1zdHlsZSBTVEstNC4zIGxpY2Vuc2UiIH0seyAiZmlsdGVycy5saWIvaGlnaHBhc3M6YXV0aG9yIjogIkp1bGl1cyBPLiBTbWl0aCBJSUkiIH0seyAiZmlsdGVycy5saWIvaGlnaHBhc3M6Y29weXJpZ2h0IjogIkNvcHlyaWdodCAoQykgMjAwMy0yMDE5IGJ5IEp1bGl1cyBPLiBTbWl0aCBJSUkgPGpvc19jY3JtYS5zdGFuZm9yZC5lZHU+IiB9LHsgImZpbHRlcnMubGliL2lpcjphdXRob3IiOiAiSnVsaXVzIE8uIFNtaXRoIElJSSIgfSx7ICJmaWx0ZXJzLmxpYi9paXI6Y29weXJpZ2h0IjogIkNvcHlyaWdodCAoQykgMjAwMy0yMDE5IGJ5IEp1bGl1cyBPLiBTbWl0aCBJSUkgPGpvc19jY3JtYS5zdGFuZm9yZC5lZHU+IiB9LHsgImZpbHRlcnMubGliL2lpcjpsaWNlbnNlIjogIk1JVC1zdHlsZSBTVEstNC4zIGxpY2Vuc2UiIH0seyAiZmlsdGVycy5saWIvbG93cGFzczBfaGlnaHBhc3MxIjogIkNvcHlyaWdodCAoQykgMjAwMy0yMDE5IGJ5IEp1bGl1cyBPLiBTbWl0aCBJSUkgPGpvc19jY3JtYS5zdGFuZm9yZC5lZHU+IiB9LHsgImZpbHRlcnMubGliL2xvd3Bhc3MwX2hpZ2hwYXNzMTphdXRob3IiOiAiSnVsaXVzIE8uIFNtaXRoIElJSSIgfSx7ICJmaWx0ZXJzLmxpYi9sb3dwYXNzOmF1dGhvciI6ICJKdWxpdXMgTy4gU21pdGggSUlJIiB9LHsgImZpbHRlcnMubGliL2xvd3Bhc3M6Y29weXJpZ2h0IjogIkNvcHlyaWdodCAoQykgMjAwMy0yMDE5IGJ5IEp1bGl1cyBPLiBTbWl0aCBJSUkgPGpvc19jY3JtYS5zdGFuZm9yZC5lZHU+IiB9LHsgImZpbHRlcnMubGliL2xvd3Bhc3M6bGljZW5zZSI6ICJNSVQtc3R5bGUgU1RLLTQuMyBsaWNlbnNlIiB9LHsgImZpbHRlcnMubGliL2xwdE46YXV0aG9yIjogIkp1bGl1cyBPLiBTbWl0aCBJSUkiIH0seyAiZmlsdGVycy5saWIvbHB0Tjpjb3B5cmlnaHQiOiAiQ29weXJpZ2h0IChDKSAyMDAzLTIwMTkgYnkgSnVsaXVzIE8uIFNtaXRoIElJSSA8am9zX2Njcm1hLnN0YW5mb3JkLmVkdT4iIH0seyAiZmlsdGVycy5saWIvbHB0TjpsaWNlbnNlIjogIk1JVC1zdHlsZSBTVEstNC4zIGxpY2Vuc2UiIH0seyAiZmlsdGVycy5saWIvbmFtZSI6ICJGYXVzdCBGaWx0ZXJzIExpYnJhcnkiIH0seyAiZmlsdGVycy5saWIvdGYxOmF1dGhvciI6ICJKdWxpdXMgTy4gU21pdGggSUlJIiB9LHsgImZpbHRlcnMubGliL3RmMTpjb3B5cmlnaHQiOiAiQ29weXJpZ2h0IChDKSAyMDAzLTIwMTkgYnkgSnVsaXVzIE8uIFNtaXRoIElJSSA8am9zX2Njcm1hLnN0YW5mb3JkLmVkdT4iIH0seyAiZmlsdGVycy5saWIvdGYxOmxpY2Vuc2UiOiAiTUlULXN0eWxlIFNUSy00LjMgbGljZW5zZSIgfSx7ICJmaWx0ZXJzLmxpYi90ZjFzOmF1dGhvciI6ICJKdWxpdXMgTy4gU21pdGggSUlJIiB9LHsgImZpbHRlcnMubGliL3RmMXM6Y29weXJpZ2h0IjogIkNvcHlyaWdodCAoQykgMjAwMy0yMDE5IGJ5IEp1bGl1cyBPLiBTbWl0aCBJSUkgPGpvc19jY3JtYS5zdGFuZm9yZC5lZHU+IiB9LHsgImZpbHRlcnMubGliL3RmMXM6bGljZW5zZSI6ICJNSVQtc3R5bGUgU1RLLTQuMyBsaWNlbnNlIiB9LHsgImZpbHRlcnMubGliL3RmMjphdXRob3IiOiAiSnVsaXVzIE8uIFNtaXRoIElJSSIgfSx7ICJmaWx0ZXJzLmxpYi90ZjI6Y29weXJpZ2h0IjogIkNvcHlyaWdodCAoQykgMjAwMy0yMDE5IGJ5IEp1bGl1cyBPLiBTbWl0aCBJSUkgPGpvc19jY3JtYS5zdGFuZm9yZC5lZHU+IiB9LHsgImZpbHRlcnMubGliL3RmMjpsaWNlbnNlIjogIk1JVC1zdHlsZSBTVEstNC4zIGxpY2Vuc2UiIH0seyAiZmlsdGVycy5saWIvdGYyczphdXRob3IiOiAiSnVsaXVzIE8uIFNtaXRoIElJSSIgfSx7ICJmaWx0ZXJzLmxpYi90ZjJzOmNvcHlyaWdodCI6ICJDb3B5cmlnaHQgKEMpIDIwMDMtMjAxOSBieSBKdWxpdXMgTy4gU21pdGggSUlJIDxqb3NfY2NybWEuc3RhbmZvcmQuZWR1PiIgfSx7ICJmaWx0ZXJzLmxpYi90ZjJzOmxpY2Vuc2UiOiAiTUlULXN0eWxlIFNUSy00LjMgbGljZW5zZSIgfSx7ICJmaWx0ZXJzLmxpYi92ZXJzaW9uIjogIjAuMyIgfSx7ICJtYXRocy5saWIvYXV0aG9yIjogIkdSQU1FIiB9LHsgIm1hdGhzLmxpYi9jb3B5cmlnaHQiOiAiR1JBTUUiIH0seyAibWF0aHMubGliL2xpY2Vuc2UiOiAiTEdQTCB3aXRoIGV4Y2VwdGlvbiIgfSx7ICJtYXRocy5saWIvbmFtZSI6ICJGYXVzdCBNYXRoIExpYnJhcnkiIH0seyAibWF0aHMubGliL3ZlcnNpb24iOiAiMi41IiB9LHsgIm5hbWUiOiAiZmF1c3RnZW4tMyIgfSx7ICJvc2NpbGxhdG9ycy5saWIvbGZfc2F3cG9zOmF1dGhvciI6ICJCYXJ0IEJyb3VucywgcmV2aXNlZCBieSBTdMOpcGhhbmUgTGV0eiIgfSx7ICJvc2NpbGxhdG9ycy5saWIvbGZfc2F3cG9zOmxpY2VuY2UiOiAiU1RLLTQuMyIgfSx7ICJvc2NpbGxhdG9ycy5saWIvbmFtZSI6ICJGYXVzdCBPc2NpbGxhdG9yIExpYnJhcnkiIH0seyAib3NjaWxsYXRvcnMubGliL3ZlcnNpb24iOiAiMC4zIiB9LHsgInBsYXRmb3JtLmxpYi9uYW1lIjogIkdlbmVyaWMgUGxhdGZvcm0gTGlicmFyeSIgfSx7ICJwbGF0Zm9ybS5saWIvdmVyc2lvbiI6ICIwLjIiIH0seyAicm91dGVzLmxpYi9uYW1lIjogIkZhdXN0IFNpZ25hbCBSb3V0aW5nIExpYnJhcnkiIH0seyAicm91dGVzLmxpYi92ZXJzaW9uIjogIjAuMiIgfSx7ICJzaWduYWxzLmxpYi9uYW1lIjogIkZhdXN0IFNpZ25hbCBSb3V0aW5nIExpYnJhcnkiIH0seyAic2lnbmFscy5saWIvdmVyc2lvbiI6ICIwLjMiIH1dLCJ1aSI6IFsgeyJ0eXBlIjogInZncm91cCIsImxhYmVsIjogImZhdXN0Z2VuLTMiLCJpdGVtcyI6IFtdfV19AC50ZXh0AAAAAAAAAAEAAAADAXEMAAAnAAAADT9orAEAAAAAAC5kYXRhAAAAAAAAAAIAAAADAQAAAAAAAAAAAAAAAAIAAAAAAC5ic3MAAAAAAAAAAAMAAAADAQAAAAAAAAAAAAAAAAMAAAAAAC5yZGF0YQAAAAAAAAQAAAADAQgAAAAAAAAAgMzaogQAAgAAAAAA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AAAyFQAAoAQAAAEAIAACAAAAAADfFAAAsAQAAAEAIAACAAAAAABAFQAAwAQAAAEAIAACAAAAAAAEAAAAAAAAACMAAAADAAAAAAAlFQAA0AQAAAEAIAACAF9mbHR1c2VkAAAAAAAAAAACAC5maWxlAAAAAAAAAP7/AABnBC1sYW5nIGxsdm0gMTAuMC4wIC1lcyAxIC1tY2QgMTYgLWRvdWJsZSAtZnR6IDAsIHYyLjQxLjEAAAAAAAAAAAAAAAAAAAAAAL4YAAB7Im5hbWUiOiAiZmF1c3RnZW4tMyIsImZpbGVuYW1lIjogImZhdXN0Z2VuLTMiLCJ2ZXJzaW9uIjogIjIuNDEuMSIsImNvbXBpbGVfb3B0aW9ucyI6ICItbGFuZyBsbHZtIDEwLjAuMCAtZXMgMSAtbWNkIDE2IC1kb3VibGUgLWZ0eiAwIiwibGlicmFyeV9saXN0IjogWyJDOlxVc2Vyc1xwZWNxdVxEb2N1bWVudHNcTWF4IDhcUGFja2FnZXNcZmF1c3RnZW5cZXh0ZXJuYWxzXG1zcFxmYXVzdGdlbi1yZXNvdXJjZXMvc3RkZmF1c3QubGliIiwiQzpcVXNlcnNccGVjcXVcRG9jdW1lbnRzXE1heCA4XFBhY2thZ2VzXGZhdXN0Z2VuXGV4dGVybmFsc1xtc3BcZmF1c3RnZW4tcmVzb3VyY2VzL2FuYWx5emVycy5saWIiLCJDOlxVc2Vyc1xwZWNxdVxEb2N1bWVudHNcTWF4IDhcUGFja2FnZXNcZmF1c3RnZW5cZXh0ZXJuYWxzXG1zcFxmYXVzdGdlbi1yZXNvdXJjZXMvZW52ZWxvcGVzLmxpYiIsIkM6XFVzZXJzXHBlY3F1XERvY3VtZW50c1xNYXggOFxQYWNrYWdlc1xmYXVzdGdlblxleHRlcm5hbHNcbXNwXGZhdXN0Z2VuLXJlc291cmNlcy9zaWduYWxzLmxpYiIsIkM6XFVzZXJzXHBlY3F1XERvY3VtZW50c1xNYXggOFxQYWNrYWdlc1xmYXVzdGdlblxleHRlcm5hbHNcbXNwXGZhdXN0Z2VuLXJlc291cmNlcy9tYXRocy5saWIiLCJDOlxVc2Vyc1xwZWNxdVxEb2N1bWVudHNcTWF4IDhcUGFja2FnZXNcZmF1c3RnZW5cZXh0ZXJuYWxzXG1zcFxmYXVzdGdlbi1yZXNvdXJjZXMvcGxhdGZvcm0ubGliIiwiQzpcVXNlcnNccGVjcXVcRG9jdW1lbnRzXE1heCA4XFBhY2thZ2VzXGZhdXN0Z2VuXGV4dGVybmFsc1xtc3BcZmF1c3RnZW4tcmVzb3VyY2VzL2Jhc2ljcy5saWIiLCJDOlxVc2Vyc1xwZWNxdVxEb2N1bWVudHNcTWF4IDhcUGFja2FnZXNcZmF1c3RnZW5cZXh0ZXJuYWxzXG1zcFxmYXVzdGdlbi1yZXNvdXJjZXMvb3NjaWxsYXRvcnMubGliIiwiQzpcVXNlcnNccGVjcXVcRG9jdW1lbnRzXE1heCA4XFBhY2thZ2VzXGZhdXN0Z2VuXGV4dGVybmFsc1xtc3BcZmF1c3RnZW4tcmVzb3VyY2VzL2ZpbHRlcnMubGliIiwiQzpcVXNlcnNccGVjcXVcRG9jdW1lbnRzXE1heCA4XFBhY2thZ2VzXGZhdXN0Z2VuXGV4dGVybmFsc1xtc3BcZmF1c3RnZW4tcmVzb3VyY2VzL3JvdXRlcy5saWIiXSwiaW5jbHVkZV9wYXRobmFtZXMiOiBbIkM6XFVzZXJzXHBlY3F1XERvY3VtZW50c1xNYXggOFxQYWNrYWdlc1xmYXVzdGdlblxleHRlcm5hbHNcbXNwXGZhdXN0Z2VuLXJlc291cmNlc1wiLCJDOlxQcm9ncmFtIEZpbGVzXEN5Y2xpbmcgJzc0L3NoYXJlL2ZhdXN0IiwiL3Vzci9sb2NhbC9zaGFyZS9mYXVzdCIsIi91c3Ivc2hhcmUvZmF1c3QiLCIuIl0sInNpemUiOiAzMzMyMCwiaW5wdXRzIjogMSwib3V0cHV0cyI6IDEsInNyX2luZGV4IjogMCwibWV0YSI6IFsgeyAiYW5hbHl6ZXJzLmxpYi9uYW1lIjogIkZhdXN0IEFuYWx5emVyIExpYnJhcnkiIH0seyAiYW5hbHl6ZXJzLmxpYi9waXRjaFRyYWNrZXI6YXV0aG9yIjogIkRhcmlvIFNhbmZpbGlwcG8iIH0seyAiYW5hbHl6ZXJzLmxpYi9waXRjaFRyYWNrZXI6Y29weXJpZ2h0IjogIkNvcHlyaWdodCAoQykgMjAyMiBEYXJpbyBTYW5maWxpcHBvICAgICAgIDxzYW5maWxpcHBvLmRhcmlvX2dtYWlsLmNvbT4iIH0seyAiYW5hbHl6ZXJzLmxpYi9waXRjaFRyYWNrZXI6bGljZW5zZSI6ICJNSVQgTGljZW5zZSIgfSx7ICJhbmFseXplcnMubGliL3ZlcnNpb24iOiAiMC4yIiB9LHsgImFuYWx5emVycy5saWIvemNyOmF1dGhvciI6ICJEYXJpbyBTYW5maWxpcHBvIiB9LHsgImFuYWx5emVycy5saWIvemNyOmNvcHlyaWdodCI6ICJDb3B5cmlnaHQgKEMpIDIwMjAgRGFyaW8gU2FuZmlsaXBwbyAgICAgICA8c2FuZmlsaXBwby5kYXJpb19nbWFpbC5jb20+IiB9LHsgImFuYWx5emVycy5saWIvemNyOmxpY2Vuc2UiOiAiTUlULXN0eWxlIFNUSy00LjMgbGljZW5zZSIgfSx7ICJiYXNpY3MubGliL25hbWUiOiAiRmF1c3QgQmFzaWMgRWxlbWVudCBMaWJyYXJ5IiB9LHsgImJhc2ljcy5saWIvdmVyc2lvbiI6ICIwLjgiIH0seyAiY29tcGlsZV9vcHRpb25zIjogIi1sYW5nIGxsdm0gMTAuMC4wIC1lcyAxIC1tY2QgMTYgLWRvdWJsZSAtZnR6IDAiIH0seyAiZW52ZWxvcGVzLmxpYi9hZHNyOmF1dGhvciI6ICJZYW5uIE9ybGFyZXkgYW5kIEFuZHJleSBCdW5kaW4iIH0seyAiZW52ZWxvcGVzLmxpYi9hdXRob3IiOiAiR1JBTUUiIH0seyAiZW52ZWxvcGVzLmxpYi9jb3B5cmlnaHQiOiAiR1JBTUUiIH0seyAiZW52ZWxvcGVzLmxpYi9saWNlbnNlIjogIkxHUEwgd2l0aCBleGNlcHRpb24iIH0seyAiZW52ZWxvcGVzLmxpYi9uYW1lIjogIkZhdXN0IEVudmVsb3BlIExpYnJhcnkiIH0seyAiZW52ZWxvcGVzLmxpYi92ZXJzaW9uIjogIjAuMiIgfSx7ICJmaWxlbmFtZSI6ICJmYXVzdGdlbi0zIiB9LHsgImZpbHRlcnMubGliL2ZpcjphdXRob3IiOiAiSnVsaXVzIE8uIFNtaXRoIElJSSIgfSx7ICJmaWx0ZXJzLmxpYi9maXI6Y29weXJpZ2h0IjogIkNvcHlyaWdodCAoQykgMjAwMy0yMDE5IGJ5IEp1bGl1cyBPLiBTbWl0aCBJSUkgPGpvc19jY3JtYS5zdGFuZm9yZC5lZHU+IiB9LHsgImZpbHRlcnMubGliL2ZpcjpsaWNlbnNlIjogIk1JVC1zdHlsZSBTVEstNC4zIGxpY2Vuc2UiIH0seyAiZmlsdGVycy5saWIvaGlnaHBhc3M6YXV0aG9yIjogIkp1bGl1cyBPLiBTbWl0aCBJSUkiIH0seyAiZmlsdGVycy5saWIvaGlnaHBhc3M6Y29weXJpZ2h0IjogIkNvcHlyaWdodCAoQykgMjAwMy0yMDE5IGJ5IEp1bGl1cyBPLiBTbWl0aCBJSUkgPGpvc19jY3JtYS5zdGFuZm9yZC5lZHU+IiB9LHsgImZpbHRlcnMubGliL2lpcjphdXRob3IiOiAiSnVsaXVzIE8uIFNtaXRoIElJSSIgfSx7ICJmaWx0ZXJzLmxpYi9paXI6Y29weXJpZ2h0IjogIkNvcHlyaWdodCAoQykgMjAwMy0yMDE5IGJ5IEp1bGl1cyBPLiBTbWl0aCBJSUkgPGpvc19jY3JtYS5zdGFuZm9yZC5lZHU+IiB9LHsgImZpbHRlcnMubGliL2lpcjpsaWNlbnNlIjogIk1JVC1zdHlsZSBTVEstNC4zIGxpY2Vuc2UiIH0seyAiZmlsdGVycy5saWIvbG93cGFzczBfaGlnaHBhc3MxIjogIkNvcHlyaWdodCAoQykgMjAwMy0yMDE5IGJ5IEp1bGl1cyBPLiBTbWl0aCBJSUkgPGpvc19jY3JtYS5zdGFuZm9yZC5lZHU+IiB9LHsgImZpbHRlcnMubGliL2xvd3Bhc3MwX2hpZ2hwYXNzMTphdXRob3IiOiAiSnVsaXVzIE8uIFNtaXRoIElJSSIgfSx7ICJmaWx0ZXJzLmxpYi9sb3dwYXNzOmF1dGhvciI6ICJKdWxpdXMgTy4gU21pdGggSUlJIiB9LHsgImZpbHRlcnMubGliL2xvd3Bhc3M6Y29weXJpZ2h0IjogIkNvcHlyaWdodCAoQykgMjAwMy0yMDE5IGJ5IEp1bGl1cyBPLiBTbWl0aCBJSUkgPGpvc19jY3JtYS5zdGFuZm9yZC5lZHU+IiB9LHsgImZpbHRlcnMubGliL2xvd3Bhc3M6bGljZW5zZSI6ICJNSVQtc3R5bGUgU1RLLTQuMyBsaWNlbnNlIiB9LHsgImZpbHRlcnMubGliL2xwdE46YXV0aG9yIjogIkp1bGl1cyBPLiBTbWl0aCBJSUkiIH0seyAiZmlsdGVycy5saWIvbHB0Tjpjb3B5cmlnaHQiOiAiQ29weXJpZ2h0IChDKSAyMDAzLTIwMTkgYnkgSnVsaXVzIE8uIFNtaXRoIElJSSA8am9zX2Njcm1hLnN0YW5mb3JkLmVkdT4iIH0seyAiZmlsdGVycy5saWIvbHB0TjpsaWNlbnNlIjogIk1JVC1zdHlsZSBTVEstNC4zIGxpY2Vuc2UiIH0seyAiZmlsdGVycy5saWIvbmFtZSI6ICJGYXVzdCBGaWx0ZXJzIExpYnJhcnkiIH0seyAiZmlsdGVycy5saWIvdGYxOmF1dGhvciI6ICJKdWxpdXMgTy4gU21pdGggSUlJIiB9LHsgImZpbHRlcnMubGliL3RmMTpjb3B5cmlnaHQiOiAiQ29weXJpZ2h0IChDKSAyMDAzLTIwMTkgYnkgSnVsaXVzIE8uIFNtaXRoIElJSSA8am9zX2Njcm1hLnN0YW5mb3JkLmVkdT4iIH0seyAiZmlsdGVycy5saWIvdGYxOmxpY2Vuc2UiOiAiTUlULXN0eWxlIFNUSy00LjMgbGljZW5zZSIgfSx7ICJmaWx0ZXJzLmxpYi90ZjFzOmF1dGhvciI6ICJKdWxpdXMgTy4gU21pdGggSUlJIiB9LHsgImZpbHRlcnMubGliL3RmMXM6Y29weXJpZ2h0IjogIkNvcHlyaWdodCAoQykgMjAwMy0yMDE5IGJ5IEp1bGl1cyBPLiBTbWl0aCBJSUkgPGpvc19jY3JtYS5zdGFuZm9yZC5lZHU+IiB9LHsgImZpbHRlcnMubGliL3RmMXM6bGljZW5zZSI6ICJNSVQtc3R5bGUgU1RLLTQuMyBsaWNlbnNlIiB9LHsgImZpbHRlcnMubGliL3RmMjphdXRob3IiOiAiSnVsaXVzIE8uIFNtaXRoIElJSSIgfSx7ICJmaWx0ZXJzLmxpYi90ZjI6Y29weXJpZ2h0IjogIkNvcHlyaWdodCAoQykgMjAwMy0yMDE5IGJ5IEp1bGl1cyBPLiBTbWl0aCBJSUkgPGpvc19jY3JtYS5zdGFuZm9yZC5lZHU+IiB9LHsgImZpbHRlcnMubGliL3RmMjpsaWNlbnNlIjogIk1JVC1zdHlsZSBTVEstNC4zIGxpY2Vuc2UiIH0seyAiZmlsdGVycy5saWIvdGYyczphdXRob3IiOiAiSnVsaXVzIE8uIFNtaXRoIElJSSIgfSx7ICJmaWx0ZXJzLmxpYi90ZjJzOmNvcHlyaWdodCI6ICJDb3B5cmlnaHQgKEMpIDIwMDMtMjAxOSBieSBKdWxpdXMgTy4gU21pdGggSUlJIDxqb3NfY2NybWEuc3RhbmZvcmQuZWR1PiIgfSx7ICJmaWx0ZXJzLmxpYi90ZjJzOmxpY2Vuc2UiOiAiTUlULXN0eWxlIFNUSy00LjMgbGljZW5zZSIgfSx7ICJmaWx0ZXJzLmxpYi92ZXJzaW9uIjogIjAuMyIgfSx7ICJtYXRocy5saWIvYXV0aG9yIjogIkdSQU1FIiB9LHsgIm1hdGhzLmxpYi9jb3B5cmlnaHQiOiAiR1JBTUUiIH0seyAibWF0aHMubGliL2xpY2Vuc2UiOiAiTEdQTCB3aXRoIGV4Y2VwdGlvbiIgfSx7ICJtYXRocy5saWIvbmFtZSI6ICJGYXVzdCBNYXRoIExpYnJhcnkiIH0seyAibWF0aHMubGliL3ZlcnNpb24iOiAiMi41IiB9LHsgIm5hbWUiOiAiZmF1c3RnZW4tMyIgfSx7ICJvc2NpbGxhdG9ycy5saWIvbGZfc2F3cG9zOmF1dGhvciI6ICJCYXJ0IEJyb3VucywgcmV2aXNlZCBieSBTdMOpcGhhbmUgTGV0eiIgfSx7ICJvc2NpbGxhdG9ycy5saWIvbGZfc2F3cG9zOmxpY2VuY2UiOiAiU1RLLTQuMyIgfSx7ICJvc2NpbGxhdG9ycy5saWIvbmFtZSI6ICJGYXVzdCBPc2NpbGxhdG9yIExpYnJhcnkiIH0seyAib3NjaWxsYXRvcnMubGliL3ZlcnNpb24iOiAiMC4zIiB9LHsgInBsYXRmb3JtLmxpYi9uYW1lIjogIkdlbmVyaWMgUGxhdGZvcm0gTGlicmFyeSIgfSx7ICJwbGF0Zm9ybS5saWIvdmVyc2lvbiI6ICIwLjIiIH0seyAicm91dGVzLmxpYi9uYW1lIjogIkZhdXN0IFNpZ25hbCBSb3V0aW5nIExpYnJhcnkiIH0seyAicm91dGVzLmxpYi92ZXJzaW9uIjogIjAuMiIgfSx7ICJzaWduYWxzLmxpYi9uYW1lIjogIkZhdXN0IFNpZ25hbCBSb3V0aW5nIExpYnJhcnkiIH0seyAic2lnbmFscy5saWIvdmVyc2lvbiI6ICIwLjMiIH1dLCJ1aSI6IFsgeyJ0eXBlIjogInZncm91cCIsImxhYmVsIjogImZhdXN0Z2VuLTMiLCJpdGVtcyI6IFtdfV19AGRlc3Ryb3lteWRzcABjbGFzc0luaXRteWRzcABpbnN0YW5jZUNvbnN0YW50c215ZHNwAGluc3RhbmNlQ2xlYXJteWRzcABjb21wdXRlbXlkc3AAYWxsb2NhdGVteWRzcABnZXRKU09ObXlkc3AAX194bW1AN2ZmZmZmZmZmZmZmZmZmZjdmZmZmZmZmZmZmZmZmZmYAX19yZWFsQDQwNWY2YTdhMjk1NTM4NWUAX19yZWFsQDQwNGY2YTdhMjk1NTM4NWUAX19yZWFsQDQwNDYwY2NjY2NjY2NjY2QAX19yZWFsQGJmZjZhMDllNjY3ZjNiY2MAX19yZWFsQDNmZjZhMDllNjY3ZjNiY2MAX19yZWFsQDNmODQ3YWUxNDdhZTE0N2IAX19yZWFsQDNmYjk5OTk5OTk5OTk5OWEAX19yZWFsQDQwMDkyMWZiNTQ0NDJkMTgAX19yZWFsQDQwMzc3MmVlNWRjYmI5NzcAX19yZWFsQDQwYjI2ODYzOTQzN2VmMDcAX19yZWFsQGMwNTkwMDAwMDAwMDZlYTYAX19yZWFsQDNmZTY2NjY2NjY2NjY2NjYAX19yZWFsQGJmZmQ5MDZiY2YzMjhkNDYAX19yZWFsQDNmZmQ5MDZiY2YzMjhkNDYAX194bW1AM2ZmMDAwMDAwMDAwMDAwMDNmZDMzMzMzMzMzMzMzMzQAX19yZWFsQGJmZTg3ZGUyYTZhZWE5NjIAX19yZWFsQDNmZTg3ZGUyYTZhZWE5NjIAX19yZWFsQDQwOWZmYzAwMDAwMDAwMDAAX19yZWFsQDQxMDc3MDAwMDAwMDAwMDAAX19yZWFsQGMwNTkwMDAwMDAwMDAwMDAAX19yZWFsQDQwMzQwMDAwMDAwMDAwMDAAX19yZWFsQDQwMjQwMDAwMDAwMDAwMDAAX19yZWFsQGJmZjAwMDAwMDAwMDAwMDAAX19yZWFsQDNmZjAwMDAwMDAwMDAwMDAAX195bW1AM2ZmMDAwMDAwMDAwMDAwMDNmZjAwMDAwMDAwMDAwMDBjMDAwMDAwMDAwMDAwMDAwM2ZmMDAwMDAwMDAwMDAwMABfX3htbUAzZmUwMDAwMDAwMDAwMDAwM2ZkMDAwMDAwMDAwMDAwMABfX3htbUA4MDAwMDAwMDAwMDAwMDAwODAwMDAwMDAwMDAwMDAwMABfX3JlYWxANDAwMDAwMDAwMDAwMDAwMABfX3htbUAzZmYwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMABfX3htbUA0MDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMAA=",
					"machinecode_size" : 25508,
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 3,
					"outlettype" : [ "signal", "", "" ],
					"patching_rect" : [ 495.605258464813232, 242.342105031013489, 202.0, 22.0 ],
					"sample_format" : 1,
					"serial_number" : "DESKTOP-2836UNJ64 bits",
					"sourcecode" : "// Try guitar signal to control an oscillator.\n// First version : direct magnetic pickup, monophonic detection\n\nimport(\"stdfaust.lib\");\n\n\n//<: ((an.pitchTracker(2,0.01) : os.square),\n\n//pitchToOsc = (+<:(_, an.amp_follower(0.01)) : * : an.pitchTracker(2,0.01))~(os.osc : *(0)) : os.osc;\npitchToOsc = fi.lowpass(4, 1500) : fi.highpass(4, 40) <: *(an.amp_follower(0.01)) : an.pitchTracker(2,0.01) : os.square;\nampToEnv = an.amp_follower(0.01) : *(10);\n\nprocess = _ <: pitchToOsc, ((ampToEnv>0.01) : en.adsr(0.01,0.1,0.7,0.1) : si.smoo ) : si.dot(1) ;//*(_ : ampToEnv) ;",
					"sourcecode_size" : 568,
					"text" : "faustgen~",
					"varname" : "faustgen-633227728",
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
					"destination" : [ "obj-1", 0 ],
					"order" : 0,
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"order" : 1,
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
				"bootpath" : "~/Music",
				"type" : "WAVE",
				"implicit" : 1
			}
, 			{
				"name" : "EQ Curve Analyzer.maxsnap",
				"bootpath" : "~/Documents/Max 8/Snapshots",
				"type" : "mx@s",
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
