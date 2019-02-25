
import sys
import os
import copy
import time

class PanelDesignManagerServer:

	def __init__(self):
		pass
	#def init
	
	def startup(self,options):
		
		self.internal_variable=copy.deepcopy(objects["VariablesManager"].get_variable("PANELDESIGNMANAGER"))
		
		if self.internal_variable==None:
			try:
				self.initialize_variable()
				objects["VariablesManager"].add_variable("PANELDESIGNMANAGER",copy.deepcopy(self.internal_variable),"","Panel Design Manager internal variable","n4d-panel-design-manager-server")
			except Exception as e:
				print e
	
	#def startup
	
	def initialize_variable(self):
		
		self.internal_variable={}
		self.internal_variable["status"]=False
		self.internal_variable["kconfig"]=None
		self.internal_variable["date"]=None
		self.internal_variable["replicate"]=True
		
	#def initialize_variable
	
	def check_variable(self,variable):

		try:
			if not type(variable)==dict:
				return False
			if not type(variable["status"])==bool:
				return False
			if not type(variable["kconfig"])==dict:
				return False
			if not type(variable["date"])==str:
				return False
			if not type(variable["replicate"])==bool:
				return False
		except:
			return False

		return True
		
	#def check_variable
	
	def save_configuration(self,data):
		
		if not self.check_variable(data):
			return {"status":False,"msg":"Variable does not have the expected structure"}
		
		#write local kconfig here?
		
		self.internal_variable=copy.deepcopy(data)
		objects["VariablesManager"].set_variable("PANELDESIGNMANAGER",data)
		
		return {"status":True,"msg":""}
		
	#def save_configuration
	
	def get_configuration(self):
		
		return {"status":True,"msg":self.internal_variable}
		
	#def load_configuration


