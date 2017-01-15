<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE xsl:stylesheet [<!ENTITY nl "&#xa;">]><!--new line-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="text" encoding="utf-8"/>
<xsl:key name="mediatype" match="Page" use="@form" />

<xsl:template match="/">
    <xsl:text>Pocet obalok: </xsl:text>
    <xsl:value-of select="count(//Envelope)"/>

    <xsl:text>&nl;Pocet stran: </xsl:text>
    <xsl:value-of select="count(//Page)"/>

    <xsl:text>&nl;&nl;Pocty pouzitych typov papiera</xsl:text>
    <xsl:text>&nl;-----------------------------&nl;</xsl:text>

    <xsl:for-each select="//Page[count(. | key('mediatype', @form)[1]) = 1]">
        <xsl:sort select="@tray" />
        <xsl:value-of select="@form" />
        <xsl:text> (tray </xsl:text>
        <xsl:value-of select="@tray" />
        <xsl:text>, seq X): </xsl:text>
        <xsl:value-of select="count(key('mediatype', @form))" />
        <xsl:text>&nl;</xsl:text>
    </xsl:for-each>

    <xsl:text>&nl;Triedenie HSS</xsl:text>
    <xsl:text>&nl;-------------&nl;</xsl:text>
</xsl:template>
</xsl:stylesheet>
