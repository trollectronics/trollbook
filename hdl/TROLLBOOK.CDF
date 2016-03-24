/* Quartus II Version 9.0 Build 235 06/17/2009 Service Pack 2 SJ Web Edition */
JedecChain;
	FileRevision(JESD32A);
	DefaultMfr(6E);

	P ActionCode(Cfg)
		Device PartName(EPF10K50VR240) Path("G:/trollbook/hdl/") File("trollbook.sof") MfrSpec(OpMask(1));
	P ActionCode(Ign)
		Device PartName(EPC2) MfrSpec(OpMask(0));

ChainEnd;

AlteraBegin;
	ChainType(JTAG);
AlteraEnd;
