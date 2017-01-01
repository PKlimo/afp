<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="text" encoding="utf-8"/>

  <xsl:template match="/">
	  <xsl:apply-templates select="//StructuredField"/>
</xsl:template>

  <xsl:template match="StructuredField">
	  <xsl:value-of select="Data"/>
	  <!--xsl:text>;</xsl:text>
      <xsl:value-of select="StructuredFieldIntroducer/SFType"/>
    <xsl:text>
</xsl:text><!-put new line here-->
  </xsl:template>

</xsl:stylesheet>
