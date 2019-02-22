#!/usr/bin/env python

import os
import threading
import tempfile
import time

class PanelDesignManagerClient:
	
	def __init__(self):
		pass
	#def init
	
	def startup(self,options):
		
		t = threading.Thread(target=self._startup)
		t.daemon = True
		t.start()
		
	#def startup
	
	def _startup(self):
		
		try:
			
			if objects.has_key("VariablesManager"):
				objects["VariablesManager"].register_trigger("PANELDESIGNMANAGER","PanelDesignManagerClient",self.design_trigger)
			
			self.get_value_and_execute()
			
		except Exception as e:
			print (str(e))
			
	#def _startup
	
	def get_value_and_execute(self):

		# Making sure we're able to read PANELDESIGNMANAGER var from server
		tries=10
		var=None
		for x in range(0,tries):
			var=objects["VariablesManager"].get_variable("PANELDESIGNMANAGER")
			
			if var != None:
				self.design_trigger(var)
				break
			else:
				time.sleep(1)
				
	#def get_value
	
	def design_trigger(self,data):
		
		if data["status"] and data["replicate"]:
			#self._write_dconf_file(data)
		else:
			#self._delete_dconf_file()

	#def design_trigger
	
	