<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="utf-8" omit-xml-declaration="no"/>
  <xsl:param name="zml"/>
  
  <xsl:template match="StructuredField">    
      <xsl:if test="StructuredFieldIntroducer/SFType != 'XXX'">
      <xsl:copy-of select="."/>
    </xsl:if>
  </xsl:template>
  
  <xsl:template match="*">
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <!--skopiruj atributy selectnuteho nodu-->
      <xsl:apply-templates/>
      <!--aplikuj template na vsetky deti-->
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
